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
#ifndef _MODULE_WRAPPER_H_
#define _MODULE_WRAPPER_H_

#include "module_library_if.h"
#include "module_configuration.h"
#include <inttypes.h>

namespace osm_diff_watcher
{
  class module_wrapper
  {
  public:
    module_wrapper(osm_diff_analyzer_if::module_library_if::t_register_function p_func);
    const std::string & get_api_version(void)const;
    const osm_diff_analyzer_if::analyzer_description_if * get_description(void)const;
    osm_diff_analyzer_if::general_analyzer_if* create_analyzer(const osm_diff_analyzer_if::module_configuration * p_conf);
    ~module_wrapper(void);
  private:
    std::string m_api_version;
    osm_diff_analyzer_if::analyzer_description_if * m_description;
    osm_diff_analyzer_if::module_library_if::t_create_analyzer m_create;
    osm_diff_analyzer_if::module_library_if::t_cleanup m_clean_up;
  };
}
#endif // _MODULE_WRAPPER_H_
//EOF
