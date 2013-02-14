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
#ifndef _CONFIGURATION_PARSER_H_
#define _CONFIGURATION_PARSER_H_

#include "dom_configuration_extractor.h"
#include "configuration.h"
#include "dom_parser.h"

namespace osm_diff_watcher
{
  class configuration_parser
  {
  public:
    inline static const configuration * parse(const std::string & p_file_name);
  private:
  };

  //----------------------------------------------------------------------------
  const configuration * configuration_parser::parse(const std::string & p_file_name)
  {
    dom_parser l_configuration_parser("osm_diff_watcher_configuration");
    dom_configuration_extractor l_extractor;
    l_configuration_parser.add_analyzer(l_extractor);
    l_configuration_parser.parse(p_file_name);
    return l_extractor.get_result();
  }

}
#endif //_CONFIGURATION_PARSER_H_
//EOF
