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
#ifndef _DOM_ANALYZER_OPERATIONS_H_
#define _DOM_ANALYZER_OPERATIONS_H_

#include "dom_analyzer_if.h"

namespace osm_diff_watcher
{
  class apply_init: public analyze_operation<osm_diff_analyzer_dom_if::dom_analyzer_if>
  {
  public:
    apply_init(const osm_diff_analyzer_if::osm_diff_state * p_state):
      m_state(p_state)
      {
      }
    void operator()(osm_diff_analyzer_dom_if::dom_analyzer_if * p_analyzer)const
    {
      p_analyzer->init(m_state);
    }
  private:
    const osm_diff_analyzer_if::osm_diff_state * m_state;
  }
  ;

 class dom_analyze_operation: public analyze_operation<osm_diff_analyzer_dom_if::dom_analyzer_if>
  {
  public:
    dom_analyze_operation(const osm_diff_analyzer_dom_if::t_dom_tree & p_tree):
      m_tree(p_tree)
      {
      }

      void operator()(osm_diff_analyzer_dom_if::dom_analyzer_if * p_analyzer)const
      {
	p_analyzer->analyze(m_tree);
      }
  private:
      const osm_diff_analyzer_dom_if::t_dom_tree & m_tree;
  }
  ;
}
#endif
//EOF
