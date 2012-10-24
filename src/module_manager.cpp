#include "module_manager.h"
#include "module_wrapper.h"

#ifndef _WIN32
#include <dlfcn.h>
#else
#include <windows.h>
#endif
#include <iostream>
#include <cstdlib>

namespace osm_diff_watcher
{
  //----------------------------------------------------------------------------
  void module_manager::load_library(const std::string & p_name)
  {
    std::map<std::string,void*>::const_iterator l_iter = m_library_handlers.find(p_name);
    if(l_iter == m_library_handlers.end())
      {
	// Opening shared library
#ifndef _WIN32
	t_lib_handler l_library_handle = dlopen (p_name.c_str(), RTLD_LOCAL | RTLD_NOW );
#else // _WIN32
	
#ifndef UNICODE
	t_lib_handler l_library_handle = (void*)LoadLibrary(p_name.c_str());
#else // UNICODE
	unsigned int l_size = MultiByteToWideChar(CP_ACP, 0, p_library_name.c_str(), -1, NULL, 0);
	WCHAR *l_buf = new WCHAR[l_size];
	MultiByteToWideChar(CP_ACP, 0,p_library_name.c_str(), -1, l_buf, l_size);
	t_lib_handler l_library_handle = (void*)LoadLibrary(l_buf);
#endif // UNICODE
#endif // _WIN32

	if(l_library_handle)
	  {
	    m_library_handlers.insert(make_pair(p_name,l_library_handle));

	    // Search for registering function
	    osm_diff_analyzer_if::module_library_if::t_register_function l_register_function ;
#ifndef _WIN32
	    l_register_function = (osm_diff_analyzer_if::module_library_if::t_register_function)dlsym(l_library_handle, m_register_function_name.c_str());
#else // _WIN32
	    l_register_function =  (osm_diff_analyzer_if::module_library_if::t_register_function)GetProcAddress((HMODULE)l_library_handle, m_register_function_name.c_str());
#endif
	    if (l_register_function == NULL)
	      {
		std::cout << "ERROR : Cannot find symbol \"" << m_register_function_name << "\" in library \"" << p_name << "\"" << std::endl;

#ifndef _WIN32
		dlclose(l_library_handle);
#else // _WIN32
                FreeLibrary((HMODULE)l_library_handle);
#endif
		exit(-1);
	      }
	    
	    register_module(l_register_function);
	    

	  }
	else
	  {
	    std::cout << "ERROR : unable to open library \"" << p_name << "\" : " << dlerror() << std::endl ;
	    exit(-1);
	  }

      }
    else
      {
	std::cout << "WARNING : Skipping already opened library \"" << p_name << "\"" << std::endl ;
      }
  }

  //----------------------------------------------------------------------------
  module_manager::~module_manager(void)
  {

    // Destroy module wrappers
    for(std::map<std::string,module_wrapper*>::iterator l_iter = m_module_wrappers.begin();
        l_iter != m_module_wrappers.end();
        ++l_iter)
      {
        std::cout << "Delete module wrapper of analyzer type \"" << l_iter->second->get_description()->get_type() << "\"" << std::endl;
        delete l_iter->second;
      }

    // Close libraries
    std::map<std::string,t_lib_handler>::iterator l_iter = m_library_handlers.begin();
    std::map<std::string,t_lib_handler>::iterator l_iter_end = m_library_handlers.end();
    while(l_iter != l_iter_end)
      {
	std::cout << "Closing library \"" << l_iter->first << "\"" << std::endl ;
          #ifndef _WIN32
          dlclose(l_iter->second);
          #else // _WIN32
          FreeLibrary((HMODULE)l_library_handle)
          #endif
	++l_iter;
      }
  }

  //----------------------------------------------------------------------------
  void module_manager::register_module(osm_diff_analyzer_if::module_library_if::t_register_function p_func)
  {
    module_wrapper * l_module_wrapper = new module_wrapper(p_func);
    const std::string & l_type = l_module_wrapper->get_description()->get_type();
    std::cout << "Successfull registration of analyzer type \"" << l_type << "\"" << std::endl ;
    m_module_wrappers.insert(make_pair(l_type,l_module_wrapper));
  }

  std::string module_manager::m_register_function_name = "register_module";
}
