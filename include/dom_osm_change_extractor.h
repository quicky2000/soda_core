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
#ifndef _DOM_OSM_CHANGE_EXTRACTOR_H_
#define _DOM_OSM_CHANGE_EXTRACTOR_H_

#include "dom_simple_analyzer_if.h"
#include "dom_generic_utilities.h"
#include "quicky_exception.h"
#include <sstream>
#include <cstring>

namespace osm_diff_watcher
{
  class dom_osm_change_extractor: public dom_simple_analyzer_if
  {
  public:
    inline dom_osm_change_extractor(void);
    // Method inherited from dom_analyzer_if
    inline void analyze(const osm_diff_analyzer_dom_if::t_dom_tree & p_tree);
    inline void init(void){};
    // end of Method inherited from dom_analyzer_if
    inline std::vector<osm_api_data_types::osm_change*> * get_result(void)const;
 
  private:
    std::vector<osm_api_data_types::osm_change*> * m_result;
  };

  //----------------------------------------------------------------------------
  dom_osm_change_extractor::dom_osm_change_extractor(void):
    m_result(NULL)
    {
    }

    //----------------------------------------------------------------------------
    std::vector<osm_api_data_types::osm_change*> * dom_osm_change_extractor::get_result(void)const
      {
        return m_result;
      }

    //----------------------------------------------------------------------------
    void dom_osm_change_extractor::analyze(const osm_diff_analyzer_dom_if::t_dom_tree & p_tree)
    {
      if(strcmp("osmChange",p_tree.getName()))
        {
          std::stringstream l_stream;
          l_stream << "Root of XML tree should be \"osmChange\" instead of \"" << p_tree.getName() << "\"" ;
          throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
        }

      int l_nb_child_node = p_tree.nChildNode();      
      m_result = new std::vector<osm_api_data_types::osm_change*>;
      for(int l_index = 0 ; l_index < l_nb_child_node ; ++l_index)
        {
          const osm_diff_analyzer_dom_if::t_dom_tree & l_node = p_tree.getChildNode(l_index);
          osm_api_data_types::osm_change::t_osm_change_type l_change_type = osm_api_data_types::osm_change::get_change_type(l_node.getName());
          int l_nb_child_object_nodes = l_node.nChildNode();
          for(int l_index_object = 0 ; l_index_object < l_nb_child_object_nodes ; ++l_index_object)
            {
              const osm_diff_analyzer_dom_if::t_dom_tree & l_object_node = l_node.getChildNode(l_index_object);
              osm_api_data_types::osm_core_element::t_osm_type l_osm_type = osm_api_data_types::osm_core_element::get_osm_type(l_object_node.getName());
              switch(l_osm_type)
                {
                case osm_api_data_types::osm_core_element::NODE :
                  m_result->push_back(new osm_api_data_types::osm_change_generic<osm_api_data_types::osm_node>(l_change_type,generic_dom_utilities<XMLNode>::extract_info<osm_api_data_types::osm_node>(l_object_node,false,l_change_type == osm_api_data_types::osm_change::DELETION)));
                  break;
                case osm_api_data_types::osm_core_element::WAY :
                  m_result->push_back(new osm_api_data_types::osm_change_generic<osm_api_data_types::osm_way>(l_change_type,generic_dom_utilities<XMLNode>::extract_info<osm_api_data_types::osm_way>(l_object_node)));
                  break;
                case osm_api_data_types::osm_core_element::RELATION :
                  m_result->push_back(new osm_api_data_types::osm_change_generic<osm_api_data_types::osm_relation>(l_change_type,generic_dom_utilities<XMLNode>::extract_info<osm_api_data_types::osm_relation>(l_object_node)));
                  break;
                default:
                  {
                    std::stringstream l_stream;
                    l_stream << "Unexpected core type value \"" << osm_api_data_types::osm_core_element::get_osm_type_str(l_osm_type) << "\"" ;
                    throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
                  }
                  break;
                }
            }
        }
    }

}
#endif // _DOM_OSM_CHANGE_EXTRACTOR_H_
//EOF
