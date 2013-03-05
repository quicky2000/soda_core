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
#ifndef _DOM_OSM_OBJECT_HISTORY_EXTRACTOR_H_
#define _DOM_OSM_OBJECT_HISTORY_EXTRACTOR_H_

#include "dom_extractor_base.h"
#include "dom_generic_utilities.h"
#include "quicky_exception.h"
#include <sstream>
#include <cstring>

namespace osm_diff_watcher
{
  template <class T>
  class dom_osm_object_history_extractor: public dom_extractor_base
  {
  public:
    dom_osm_object_history_extractor(void);
    // Method inherited from dom_analyzer_if
    void analyze(const osm_diff_analyzer_if::t_dom_tree & p_tree);
    // end of Method inherited from dom_analyzer_if
    std::vector<T*> * get_result(void)const;
  private:
    std::vector<T*> * m_result;
  }
  ;

  //----------------------------------------------------------------------------
  template <class T>
    dom_osm_object_history_extractor<T>::dom_osm_object_history_extractor(void):
    dom_extractor_base("dom_osm_object_history_extractor<"+t::get_type_str()+">"),
    m_result(NULL)
    {
    }

  //----------------------------------------------------------------------------
  template <class T>
    std::vector<T*> * dom_osm_object_history_extractor<T>::get_result(void)const
    {
      return m_result;
    }

  //----------------------------------------------------------------------------
    template <class T>
    void dom_osm_object_history_extractor<T>::analyze(const osm_diff_analyzer_if::t_dom_tree & p_tree)
    {
      if(strcmp("osm",p_tree.getName()))
        {
          std::stringstream l_stream;
          l_stream << "Root of XML tree should be \"osm\" instead of \"" << p_tree.getName() << "\"" ;
          throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
        }
      int l_nb_version = p_tree.nChildNode();
      m_result = new std::vector<T*>();
      for(int l_index = 0; l_index < l_nb_version ; ++l_index)
	{
	  osm_diff_analyzer_if::t_dom_tree l_node = p_tree.getChildNode(l_index);

          if(strcmp(l_node.getName(),T::get_type_str().c_str()))
            {
              std::stringstream l_stream;
              l_stream << "Node name \"" << l_node.getName() << "\" should be different of type name \"" << T::get_type_str() << "\"" ;
              throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
            }
	  m_result->push_back(generic_dom_utilities<osm_diff_analyzer_if::t_dom_tree>::extract_info<T>(l_node,true));
	}
    }
  
}
#endif // _DOM_OSM_OBJECT_HISTORY_EXTRACTOR_H_
//EOF
