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

#ifndef _DOM_OSM_API_CAPABILITIES_EXTRACTOR_H
#define _DOM_OSM_API_CAPABILITIES_EXTRACTOR_H

#include "dom_simple_analyzer_if.h"
#include "dom_generic_utilities.h"
#include "osm_api_capabilities.h"
#include <cstring>

namespace osm_diff_watcher
{
  class dom_osm_api_capabilities_extractor: public dom_simple_analyzer_if
  {
  public:
    inline dom_osm_api_capabilities_extractor(void);
    // Method inherited from dom_simple_analyzer_if
    inline void analyze(const osm_diff_analyzer_dom_if::t_dom_tree & p_tree);
    inline void init(void){}
    // end of Method inherited from dom_analyzer_if
    inline const osm_api_capabilities * get_result(void)const;
  private:
    osm_api_capabilities * m_result;
  };

  //----------------------------------------------------------------------------
  dom_osm_api_capabilities_extractor::dom_osm_api_capabilities_extractor(void):
    m_result(NULL)
    {
    }
 
  //----------------------------------------------------------------------------
    const osm_api_capabilities * dom_osm_api_capabilities_extractor::get_result(void)const
    {
      return m_result;
    }

  //----------------------------------------------------------------------------
    void dom_osm_api_capabilities_extractor::analyze(const osm_diff_analyzer_dom_if::t_dom_tree & p_tree)
    {
      assert(!strcmp("osm",p_tree.getName()));
      int l_nb_child_object = p_tree.nChildNode();
      assert(l_nb_child_object==1);
      const osm_diff_analyzer_dom_if::t_dom_tree & l_api_node = p_tree.getChildNode(0);
      assert(!strcmp(l_api_node.getName(),"api"));
      l_nb_child_object = l_api_node.nChildNode();
      assert(l_nb_child_object>=6);

      std::string l_version_min;
      std::string l_version_max;
      std::string l_area_maximum;
      uint64_t l_tracepoints_per_page;
      uint64_t l_waynodes_max;
      uint64_t l_changeset_max_elements;
      uint32_t l_timeout_max;
      
      for(int l_index = 0; l_index < l_nb_child_object; ++l_index)
	{
	  osm_diff_analyzer_dom_if::t_dom_tree l_node = l_api_node.getChildNode(l_index);
          std::string l_node_name = l_node.getName();
          if(l_node_name == "version")
            {
              l_version_min = generic_dom_utilities<XMLNode>::extract_attribute<const char *>(l_node,"minimum");
              l_version_max = generic_dom_utilities<XMLNode>::extract_attribute<const char *>(l_node,"maximum");
            }
          else if(l_node_name == "area")
            {
              l_area_maximum = generic_dom_utilities<XMLNode>::extract_attribute<const char *>(l_node,"maximum");
            }
          else if(l_node_name == "tracepoints")
            {
              l_tracepoints_per_page = generic_dom_utilities<XMLNode>::extract_attribute<osm_api_data_types::osm_object::t_osm_id>(l_node,"per_page");
             }
          else if(l_node_name == "waynodes")
            {
              l_waynodes_max = generic_dom_utilities<XMLNode>::extract_attribute<osm_api_data_types::osm_object::t_osm_id>(l_node,"maximum");
            }
          else if(l_node_name == "changesets")
            {
              l_changeset_max_elements = generic_dom_utilities<XMLNode>::extract_attribute<osm_api_data_types::osm_object::t_osm_id>(l_node,"maximum_elements");
            }
          else if(l_node_name == "timeout")
            {
              l_timeout_max = generic_dom_utilities<XMLNode>::extract_attribute<osm_api_data_types::osm_core_element::t_osm_version>(l_node,"seconds");
            }
          else
            {
              std::cout << "WARNING : skipping unknown API capabilities node \"" << l_node_name << "\"" << std::endl ;
            }
	}
      m_result = new osm_api_capabilities(l_version_min,
                                          l_version_max,
                                          l_area_maximum,
                                          l_tracepoints_per_page,
                                          l_waynodes_max,
                                          l_changeset_max_elements,
                                          l_timeout_max);
    }
  
};
#endif
//EOF
