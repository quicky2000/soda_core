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
#include "sax_parser.h"
#include "sax_analyzer_operations.h"
#include "quicky_exception.h"
#include <sstream>
#include <inttypes.h>
#include <iostream>
#include <fstream>
#include <assert.h>

namespace osm_diff_watcher
{
  //------------------------------------------------------------------------------
  sax_parser::sax_parser(void):
    m_parser(XML_ParserCreate(NULL))
  {
    if (!m_parser)
      {
	throw quicky_exception::quicky_runtime_exception("Couldn't allocate memory for parser",__LINE__,__FILE__);
      }

  }

  //------------------------------------------------------------------------------
  sax_parser::~sax_parser(void)
  {
    XML_ParserFree(m_parser);
  }

  //------------------------------------------------------------------------------
  void sax_parser::parse_stream(std::istream & p_stream)
  {
    XML_ParserReset(m_parser,NULL);
    XML_SetUserData(m_parser,this);
    XML_SetElementHandler(m_parser,start, end);

    this->perform_analyze(apply_sax_init(get_diff_state()));

    const uint32_t l_size = 10000;
    char l_buf[l_size];
    bool l_end = false;
    while(! (l_end = p_stream.eof()))
      {
	p_stream.read(l_buf,l_size);
	if (! XML_Parse(m_parser, l_buf,p_stream.gcount(),l_end))
	  {
	    std::stringstream l_stream;
	    l_stream << "ERROR : Parse error at line " << XML_GetCurrentLineNumber(m_parser) << " :" << XML_ErrorString(XML_GetErrorCode(m_parser)) ;
	    throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
	  }
      }
  }

  //------------------------------------------------------------------------------
  void sax_parser::start(void *p_data, const char *p_element, const char **p_attribute)
  {
    sax_parser *l_parser = static_cast<sax_parser*>(p_data);
    if(NULL == l_parser)
      {
	throw quicky_exception::quicky_logic_exception("l_parser pointer should not be NULL",__LINE__,__FILE__);
      }
    l_parser->analyze_start_element(p_element,p_attribute);

  }

  //------------------------------------------------------------------------------
  void sax_parser::end(void *p_data, const char *p_element)
  {
    sax_parser *l_parser = static_cast<sax_parser*>(p_data);
    if(NULL == l_parser)
      {
	throw quicky_exception::quicky_logic_exception("l_parser pointer should not be NULL",__LINE__,__FILE__);
      }
    l_parser->analyze_end_element(p_element);
  }

  //------------------------------------------------------------------------------
  void sax_parser::analyze_start_element(const char *p_element, const char **p_attribute)
  {
    this->perform_analyze(apply_element(p_element,&osm_diff_analyzer_sax_if::sax_analyzer_if::start_element));
    for (uint32_t i = 0; p_attribute[i]; i += 2)
      {
	this->perform_analyze(apply_attribute(p_attribute[i],p_attribute[i+1]));
      }
  }

  //------------------------------------------------------------------------------
  void sax_parser::analyze_end_element(const char *p_element)
  {
    this->perform_analyze(apply_element(p_element,&osm_diff_analyzer_sax_if::sax_analyzer_if::end_element));
  }
}
//EOF
