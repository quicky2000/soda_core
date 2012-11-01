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
#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "module_configuration.h"
#include <string>
#include <vector>
#include <map>

namespace osm_diff_watcher
{
  class configuration
  {
  public:
    inline configuration(void);
    inline void add_library(const std::string & p_name);
    inline void add_module_configuration(osm_diff_analyzer_if::module_configuration * p_configuration);
    inline void add_variable(const std::string & p_name,const std::string & p_value);
    inline const std::vector<std::string> & get_libraries(void)const;
    inline const std::vector<osm_diff_analyzer_if::module_configuration*> & get_module_configurations(void)const;
    inline const std::map<std::string,std::string> & get_variables(void)const;
    bool is_variable_defined(const std::string & p_name)const;
    const std::string get_variable(const std::string & p_name)const;
    ~configuration(void);
  private:
    std::vector<std::string> m_libraries;
    std::vector<osm_diff_analyzer_if::module_configuration * > m_module_configurations;
    std::map<std::string,std::string> m_variables;
  };

  //----------------------------------------------------------------------------
  configuration::configuration(void)
    {
    }

  //----------------------------------------------------------------------------
  configuration::~configuration(void)
  {
    for(std::vector<osm_diff_analyzer_if::module_configuration * >::iterator l_iter = m_module_configurations.begin();
        l_iter != m_module_configurations.end();
        ++l_iter)
      {
        delete *l_iter;
      }
  }

  //----------------------------------------------------------------------------
  void configuration::add_library(const std::string & p_name)
  {
    m_libraries.push_back(p_name);
  }

  //----------------------------------------------------------------------------
  void configuration::add_module_configuration(osm_diff_analyzer_if::module_configuration * p_conf)
  {
    m_module_configurations.push_back(p_conf);
  }

  //----------------------------------------------------------------------------
  void configuration::add_variable(const std::string & p_name,const std::string & p_value)
  {
    m_variables.insert(make_pair(p_name,p_value));
  }

  //----------------------------------------------------------------------------
  const std::vector<osm_diff_analyzer_if::module_configuration *> & configuration::get_module_configurations(void)const
    {
      return m_module_configurations;
    }

  //----------------------------------------------------------------------------
  const std::map<std::string,std::string> & configuration::get_variables(void)const
    {
      return m_variables;
    }

  //----------------------------------------------------------------------------
  bool configuration::is_variable_defined(const std::string & p_name)const
  {
    return m_variables.find(p_name) != m_variables.end();
  }

  //----------------------------------------------------------------------------
  const std::string configuration::get_variable(const std::string & p_name)const
    {
      std::map<std::string,std::string>::const_iterator l_iter = m_variables.find(p_name);
      return (l_iter != m_variables.end() ? l_iter->second : "");
    }

  //----------------------------------------------------------------------------
  const std::vector<std::string> & configuration::get_libraries(void)const
    {
      return m_libraries;
    }
}
#endif // _CONFIGURATION_H_
//EOF
