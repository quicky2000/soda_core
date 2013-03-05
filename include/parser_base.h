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
#ifndef _PARSER_BASE_H_
#define _PARSER_BASE_H_

#include "quicky_exception.h"
#include "gzstream.h"
#include "analyze_operation.h"
#include "parser_if.h"
#include "output_producer_if.h"
#include <set>
#include <algorithm>
#include <iostream>
#include <sstream>

namespace osm_diff_watcher
{
  template <class T>
    class parser_base:public output_producer_if<T>, public parser_if
  {
  public:
    void parse(const std::string & p_name);
  private:
    virtual void parse_stream(std::istream & p_stream)=0;

  }
  ;

#include "parser_base.tpp"
}
#endif // _PARSER_BASE_H_
//EOF
