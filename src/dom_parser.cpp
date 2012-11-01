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
#include "dom_parser.h"
#include "dom_simple_analyzer_operations.h"
#include <sstream>

using namespace std;

namespace osm_diff_watcher
{
  //------------------------------------------------------------------------------
  dom_parser::dom_parser(const std::string & p_root):
    dom_parser_base<dom_simple_analyzer_if>(p_root)
  {
  }

  //------------------------------------------------------------------------------
  void dom_parser::perform_dedicated_analyze(const XMLNode & p_node)
  {
    this->perform_analyze(dom_simple_analyze_operation(p_node));
  }

  //------------------------------------------------------------------------------
  void dom_parser::perform_init(void)
  {
    this->perform_analyze(apply_simple_init());
  }

}

//EOF
