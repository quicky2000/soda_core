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
#ifndef _DOM_OSM_OBJECT_LIST_EXTRACTOR_H_
#define _DOM_OSM_OBJECT_LIST_EXTRACTOR_H_

#include "dom_simple_analyzer_if.h"
#include "dom_generic_utilities.h"
#include <cstring>

namespace osm_diff_watcher
{
  template <class T>
  class dom_osm_object_list_extractor: public dom_simple_analyzer_if
  {
  public:
    dom_osm_object_list_extractor(void);
    // Method inherited from dom_analyzer_if
    void analyze(const osm_diff_analyzer_dom_if::t_dom_tree & p_tree);
   inline void init(void){}
     // end of Method inherited from dom_analyzer_if
    std::vector<T*> * get_result(void)const;
  private:
    std::vector<T*> * m_result;
  }
  ;

  //----------------------------------------------------------------------------
  template <class T>
    dom_osm_object_list_extractor<T>::dom_osm_object_list_extractor(void):
    m_result(NULL)
    {
    }

  //----------------------------------------------------------------------------
  template <class T>
    std::vector<T*> * dom_osm_object_list_extractor<T>::get_result(void)const
    {
      return m_result;
    }

  //----------------------------------------------------------------------------
    template <class T>
    void dom_osm_object_list_extractor<T>::analyze(const osm_diff_analyzer_dom_if::t_dom_tree & p_tree)
    {
      assert(!strcmp("osm",p_tree.getName()));
      int l_nb_version = p_tree.nChildNode();
      m_result = new std::vector<T*>();
      for(int l_index = 0; l_index < l_nb_version ; ++l_index)
	{
	  osm_diff_analyzer_dom_if::t_dom_tree l_node = p_tree.getChildNode(l_index);
	  assert(!strcmp(l_node.getName(),T::get_type_str().c_str()));
	  m_result->push_back(generic_dom_utilities<osm_diff_analyzer_dom_if::t_dom_tree>::extract_info<T>(l_node,true));
	}
    }
  
}
#endif // _DOM_OSM_OBJECT_LIST_EXTRACTOR_H_
//EOF
