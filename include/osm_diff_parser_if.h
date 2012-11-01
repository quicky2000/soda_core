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
#ifndef _DIFF_PARSER_IF_H_
#define _DIFF_PARSER_IF_H_

#include <cassert>

namespace osm_diff_analyzer_if
{
  class osm_diff_state;
}

namespace osm_diff_watcher
{
  class osm_diff_parser_if
  {
  public:
    inline osm_diff_parser_if(void);
    inline void set_diff_state(const osm_diff_analyzer_if::osm_diff_state * p_diff_state);
    inline const osm_diff_analyzer_if::osm_diff_state * get_diff_state(void)const;
  private:
    const osm_diff_analyzer_if::osm_diff_state * m_diff_state;
  };

  //----------------------------------------------------------------------------
  osm_diff_parser_if::osm_diff_parser_if(void):
    m_diff_state(NULL)
    {
    }
    //----------------------------------------------------------------------------
    void osm_diff_parser_if::set_diff_state(const osm_diff_analyzer_if::osm_diff_state * p_diff_state)
    {
      assert(p_diff_state);
      m_diff_state = p_diff_state;
    }
    //----------------------------------------------------------------------------
    const osm_diff_analyzer_if::osm_diff_state * osm_diff_parser_if::get_diff_state(void)const
      {
        return m_diff_state;
      }
}
#endif // _OSM_DIFF_PARSER_IF_H_
//EOF
