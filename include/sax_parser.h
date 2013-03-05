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
#ifndef _SAX_PARSER_H_
#define _SAX_PARSER_H_

#ifdef _WIN32
#include "expat.h"
#else
#include <expat.h>
#endif // _WIN32
#include "sax_analyzer_if.h"
#include "parser_base.h"
#include "osm_diff_parser_if.h"
#include <iostream>
#include <set>


namespace osm_diff_watcher
{
  class sax_parser: public parser_base<osm_diff_analyzer_sax_if::sax_analyzer_if>, public osm_diff_parser_if
  {
  public:
    // Callbacks for expat
    static void start(void *data, const char *el, const char **attr);
    static void end(void *data, const char *el);
    // End of callbacks

    sax_parser(void);
    ~sax_parser(void);
  
  private:
    // Virtual method inherited from parser_base
    void parse_stream(std::istream & p_stream);
    // End of pure virtual methodes

    void analyze_start_element(const char *el, const char **attr);
    void analyze_end_element(const char *el);

    XML_Parser m_parser;
  };
}
#endif /* _SAX_PARSER_H_ */
