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
#ifndef _CPP_ANALYZER_OPERATIONS_H_
#define _CPP_ANALYZER_OPERATIONS_H_

#include "cpp_analyzer_if.h"

namespace osm_diff_watcher
{
  class apply_init: public analyze_operation<osm_diff_analyzer_cpp_if::cpp_analyzer_if>
  {
  public:
    apply_init(const osm_diff_analyzer_if::osm_diff_state * p_state):
      m_state(p_state)
      {
      }
    void operator()(osm_diff_analyzer_cpp_if::cpp_analyzer_if * p_analyzer)const
    {
      p_analyzer->init(m_state);
    }
  private:
    const osm_diff_analyzer_if::osm_diff_state * m_state;
  }
  ;

 class apply_changes: public analyze_operation<osm_diff_analyzer_cpp_if::cpp_analyzer_if>
  {
  public:
    apply_changes(const std::vector<osm_api_data_types::osm_change*> & p_changes):
      m_changes(p_changes)
      {
      }

      void operator()(osm_diff_analyzer_cpp_if::cpp_analyzer_if * p_analyzer)const
      {
	p_analyzer->analyze(m_changes);
      }
  private:
      const std::vector<osm_api_data_types::osm_change*> & m_changes;
  }
  ;
}
#endif
//EOF
