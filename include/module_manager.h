/*
    This file is part of osm_diff_watcher, Openstreetmap diff analyze framework
    The aim of this software is to provided generic facilities for diff analyzis
    to allow developers to concentrate on analyze rather than diff management 
    infrastructure
    Copyright (C) 2012  Julien Thevenon ( julien_thevenon at yahoo.fr )

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#ifndef _MODULE_MANAGER_H_
#define _MODULE_MANAGER_H_

#include "module_library_if.h"
#include <string>
#include <map>
#include <iostream>
#include <cstdlib>
#include "module_wrapper.h"
#include "module_configuration.h"

namespace osm_diff_watcher
{

  class module_manager
  {				       
  public:
    void load_library(const std::string & p_name);
    template <class T>
      T * create_module(const osm_diff_analyzer_if::module_configuration * p_conf);
    ~module_manager(void);
  private:
    typedef void* t_lib_handler;

    void register_module(osm_diff_analyzer_if::module_library_if::t_register_function p_handler);

    std::map<std::string,void*> m_library_handlers;
    std::map<std::string,module_wrapper*> m_module_wrappers;
    static std::string m_register_function_name;
  };

    template <class T>
      T * module_manager::create_module(const osm_diff_analyzer_if::module_configuration * p_conf)
      {
        std::map<std::string,module_wrapper*>::iterator l_iter = m_module_wrappers.find(p_conf->get_type());
        if(l_iter == m_module_wrappers.end())
          {
            std::cout << "ERROR : Unknown module type \"" << p_conf->get_type() << "\"" << std::endl ;
            exit(-1);
          }
#ifndef FORCE_USE_OF_REINTERPRET_CAST
        T * l_result = dynamic_cast<T*>(l_iter->second->create_analyzer(p_conf));
#else
        T * l_result = reinterpret_cast<T*>(l_iter->second->create_analyzer(p_conf));
#endif // FORCE_USE_OF_REINTERPRET_CAST
        if(l_result==NULL)
          {
            std::cout << "ERROR : Creation of module \"" << p_conf->get_name() << "\" with type \"" << p_conf->get_type() << "\" has failed" << std::endl ;
            exit(-1);
          }
        return l_result;
      }
  
}
#endif // MODULE_MANAGER
//EOF
