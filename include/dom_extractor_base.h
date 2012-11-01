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
#ifndef _DOM_EXTRACTOR_BASE_H_
#define _DOM_EXTRACTOR_BASE_H_

#include "dom_analyzer_if.h"

namespace osm_diff_watcher
{
  class dom_extractor_base: public osm_diff_analyzer_dom_if::dom_analyzer_if
  {
  public:
    inline dom_extractor_base(const std::string & p_type);

    // Method inherited from dom_analyzer_if
    inline const std::string & get_input_type(void)const;
    inline const std::string & get_output_type(void)const;
    inline const std::string & get_type(void)const;
    inline const std::string & get_name(void)const;
    inline void init(const osm_diff_analyzer_if::osm_diff_state * p_diff_state){}
    // end of Method inherited from dom_analyzer_if
  private:
    const std::string m_input_type;
    const std::string m_output_type;
    const std::string m_type;
    const std::string m_name;
  };

  dom_extractor_base::dom_extractor_base(const std::string & p_type):
    m_input_type("dom"),
    m_output_type(""),
    m_type(p_type),
    m_name(p_type+"_instance")
    {
    }

    // Method inherited from dom_analyzer_if
    const std::string & dom_extractor_base::get_input_type(void)const
      {
        return m_input_type;
      }
    const std::string & dom_extractor_base::get_output_type(void)const
      {
        return m_output_type;
      }
    const std::string & dom_extractor_base::get_type(void)const
      {
        return m_type;
      }
    const std::string & dom_extractor_base::get_name(void)const
      {
        return m_name;
      }
    // end of Method inherited from dom_analyzer_if
}
#endif // _DOM_EXTRACTOR_BASE_H_
//EOF
