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
#ifndef _DOM2CPP_ANALYZER_H_
#define _DOM2CPP_ANALYZER_H_

#include "dom_analyzer_if.h"
#include "output_producer_if.h"
#include "cpp_analyzer_if.h"

namespace osm_diff_watcher
{
  class dom2cpp_analyzer: public osm_diff_analyzer_dom_if::dom_analyzer_if, public output_producer_if<osm_diff_analyzer_cpp_if::cpp_analyzer_if>
  {
  public:
    dom2cpp_analyzer(const std::string & p_name);
    // Method inherited from dom_analyzer_if
    inline const std::string & get_input_type(void)const;
    inline const std::string & get_output_type(void)const;
    inline const std::string & get_type(void)const;
    inline const std::string & get_name(void)const;
    void analyze(const osm_diff_analyzer_dom_if::t_dom_tree & p_tree);
    inline void init(const osm_diff_analyzer_if::osm_diff_state * p_diff_state);
    inline void check(void){};
    // end of Method inherited from dom_analyzer_if
  private:
    static const std::string m_input_type;
    static const std::string m_output_type;
    static const std::string m_type;
    const std::string m_name;
    const osm_diff_analyzer_if::osm_diff_state * m_diff_state;
  };

  // Method inherited from dom_analyzer_if
  //----------------------------------------------------------------------------
  const std::string & dom2cpp_analyzer::get_input_type(void)const
    {
      return m_input_type;
    }
  //----------------------------------------------------------------------------
  const std::string & dom2cpp_analyzer::get_output_type(void)const
    {
      return m_output_type;
    }
  //----------------------------------------------------------------------------
  const std::string & dom2cpp_analyzer::get_type(void)const
    {
      return m_type;
    }
  //----------------------------------------------------------------------------
  const std::string & dom2cpp_analyzer::get_name(void)const
    {
      return m_name;
    }
  //----------------------------------------------------------------------------
  void dom2cpp_analyzer::init(const osm_diff_analyzer_if::osm_diff_state * p_diff_state)
  {
    m_diff_state = p_diff_state;
  }

}
#endif
//EOF
