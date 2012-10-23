#ifndef _MODULE_MANAGER_H_
#define _MODULE_MANAGER_H_

#include "module_library_if.h"
#include <string>
#include <map>
#include <iostream>
#include <cstdlib>
#include "module_wrapper.h"

namespace osm_diff_watcher
{

  class module_manager
  {				       
  public:
    void load_library(const std::string & p_name);
    template <class T>
      T * create_module(const std::string & p_type,
                        const std::string & p_name);
    ~module_manager(void);
  private:
    typedef void* t_lib_handler;

    void register_module(osm_diff_analyzer_if::module_library_if::t_register_function p_handler);

    std::map<std::string,void*> m_library_handlers;
    std::map<std::string,module_wrapper*> m_module_wrappers;
    static std::string m_register_function_name;
  };

    template <class T>
      T * module_manager::create_module(const std::string & p_type,
                                        const std::string & p_name)
      {
        std::map<std::string,module_wrapper*>::iterator l_iter = m_module_wrappers.find(p_type);
        if(l_iter == m_module_wrappers.end())
          {
            std::cout << "ERROR : Unknown module type \"" << p_type << "\"" << std::endl ;
            exit(-1);
          }
        T * l_result = dynamic_cast<T*>(l_iter->second->create_analyzer(p_name));
        if(l_result==NULL)
          {
            std::cout << "ERROR : Creation of module \"" << p_name << "\" with type \"" << p_type << "\" has failed" << std::endl ;
            exit(-1);
          }
        return l_result;
      }
  
}
#endif // MODULE_MANAGER
//EOF
