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
#include "module_wrapper.h"
#include "module_library_if.h"
#include "common_api_wrapper.h"
#include "quicky_exception.h"
#include <cassert>
#include <iostream>
#include <sstream>

namespace osm_diff_watcher
{
  //----------------------------------------------------------------------------
  module_wrapper::module_wrapper(osm_diff_analyzer_if::module_library_if::t_register_function p_func):
    m_description(NULL),
    m_create(NULL),
    m_clean_up(NULL)
  {

    uintptr_t l_api_ptr[MODULE_LIBRARY_IF_API_SIZE];
    for(uint32_t l_index = 0 ;l_index < MODULE_LIBRARY_IF_API_SIZE ; ++l_index)
      {
        l_api_ptr[l_index] = 0;
      }
    p_func(l_api_ptr,MODULE_LIBRARY_IF_API_SIZE);

    osm_diff_analyzer_if::module_library_if::t_get_api_version l_get_api_version = (osm_diff_analyzer_if::module_library_if::t_get_api_version)l_api_ptr[osm_diff_analyzer_if::module_library_if::GET_API_VERSION];
    m_api_version = l_get_api_version();
    if(m_api_version != MODULE_LIBRARY_IF_VERSION)
      {
	std::stringstream l_stream;
	l_stream << "osm_diff_watcher was compiled with module API " << MODULE_LIBRARY_IF_VERSION << " whereas module was compiled with version " << m_api_version;
	throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
      }
    osm_diff_analyzer_if::module_library_if::t_get_api_size l_get_api_size = (osm_diff_analyzer_if::module_library_if::t_get_api_size)l_api_ptr[osm_diff_analyzer_if::module_library_if::GET_API_SIZE];
    if(MODULE_LIBRARY_IF_API_SIZE != l_get_api_size())
      {
	throw quicky_exception::quicky_logic_exception("API size of module doesn't correspond to the one of executable. Please ensure they have been compiled with the same API version",__LINE__,__FILE__);
      }
    osm_diff_analyzer_if::module_library_if::t_get_description l_get_description = (osm_diff_analyzer_if::module_library_if::t_get_description)l_api_ptr[osm_diff_analyzer_if::module_library_if::GET_DESCRIPTION];
    m_description = l_get_description();
    m_create = (osm_diff_analyzer_if::module_library_if::t_create_analyzer)l_api_ptr[osm_diff_analyzer_if::module_library_if::CREATE_ANALYZER];
    // Provide common API to module to allow him to use common features
    osm_diff_analyzer_if::module_library_if::t_require_common_api l_require_API_ptr = (osm_diff_analyzer_if::module_library_if::t_require_common_api)l_api_ptr[osm_diff_analyzer_if::module_library_if::REQUIRE_COMMON_API];
    l_require_API_ptr(common_api_wrapper::register_function);
    m_clean_up = (osm_diff_analyzer_if::module_library_if::t_cleanup)l_api_ptr[osm_diff_analyzer_if::module_library_if::CLEAN_UP];
  }

  //----------------------------------------------------------------------------
  const std::string & module_wrapper::get_api_version(void)const
  {
    return m_api_version;
  }

  //----------------------------------------------------------------------------
  const osm_diff_analyzer_if::analyzer_description_if * module_wrapper::get_description(void)const
  {
    return m_description;
  }

  //----------------------------------------------------------------------------
  osm_diff_analyzer_if::general_analyzer_if* module_wrapper::create_analyzer(const osm_diff_analyzer_if::module_configuration * p_conf)
  {
    return m_create(p_conf);
  }

  //----------------------------------------------------------------------------
  module_wrapper::~module_wrapper(void)
  {
    delete m_description;
    m_clean_up();
  }

}
//EOF
