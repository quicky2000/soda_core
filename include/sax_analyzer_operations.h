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
#ifndef _SAX_ANALYZER_OPERATIONS_H_
#define _SAX_ANALYZER_OPERATIONS_H_

#include "sax_analyzer_if.h"
#include "analyze_operation.h"

namespace osm_diff_watcher
{
  class apply_sax_init: public analyze_operation<osm_diff_analyzer_sax_if::sax_analyzer_if>
  {
  public:
    inline apply_sax_init(const osm_diff_analyzer_if::osm_diff_state * p_diff_state):
      m_state(p_diff_state)
      {
      }

    inline void operator()(osm_diff_analyzer_sax_if::sax_analyzer_if * p_analyzer)const
    {
      p_analyzer->init(m_state);
    }
  private:
    const osm_diff_analyzer_if::osm_diff_state * m_state;
  }
  ;

  class apply_element: public analyze_operation<osm_diff_analyzer_sax_if::sax_analyzer_if>
  {
  private:
    typedef void (osm_diff_analyzer_sax_if::sax_analyzer_if::* t_method_ptr)(const std::string &);
  public:
    inline apply_element(const char * p_element, t_method_ptr p_method_ptr):
      m_element(p_element),
      m_method_ptr(p_method_ptr)
	{
	}
      inline void operator()(osm_diff_analyzer_sax_if::sax_analyzer_if * p_analyzer)const
      {
	(p_analyzer->*m_method_ptr)(m_element);
      }
  private:
      const char * m_element;
      const t_method_ptr m_method_ptr;
  }
  ;

  class apply_attribute: public analyze_operation<osm_diff_analyzer_sax_if::sax_analyzer_if>
  {
  public:
    inline apply_attribute(const char * p_name,const char * p_value):
      m_name(p_name),
      m_value(p_value)
	{
	}
      inline void operator()(osm_diff_analyzer_sax_if::sax_analyzer_if * p_analyzer)const
      {
	p_analyzer->get_attribute(m_name,m_value);
      }
  private:
      const char * m_name;
      const char * m_value;
  }
  ;
}
#endif
//EOF
