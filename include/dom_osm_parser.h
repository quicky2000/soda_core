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
#ifndef _DOM_OSM_PARSER_H_
#define _DOM_OSM_PARSER_H_

#include "osm_node.h"
#include "dom_parser.h"
#include "dom_osm_extractor.h"
#include "dom_osm_object_list_extractor.h"
#include "dom_osm_full_way_extractor.h"
#include "dom_osm_full_relation_extractor.h"
#include "dom_osm_full_extractor.h"
#include "dom_osm_api_capabilities_extractor.h"

#include <vector>

namespace osm_diff_watcher
{
  class dom_osm_parser
  {
  public:
    dom_osm_parser(void);
    template <class T>
      static const T * const get(const std::string & p_xml_string);
    template <class T>
      static const std::vector<T*> * const get_list(const std::string & p_xml_string);
    inline const osm_api_data_types::osm_way * get_full_way(const std::string & p_xml_string,
                                                            std::vector<osm_api_data_types::osm_node*> & p_nodes);

    inline const osm_api_data_types::osm_relation * get_full_relation(const std::string & p_xml_string,
                                                                      std::vector<osm_api_data_types::osm_node*> & p_nodes,
                                                                      std::vector<osm_api_data_types::osm_way*> & p_ways);
    inline const void get(const std::string & p_xml_string,
                          std::vector<osm_api_data_types::osm_node*> & p_nodes,
                          std::vector<osm_api_data_types::osm_way*> & p_ways,
                          std::vector<osm_api_data_types::osm_relation*> & p_relations);

    inline const osm_api_capabilities * get_capabilities(const std::string & p_xml_string);
  private:
  }
  ;

  //----------------------------------------------------------------------------
  template <class T>
    const T * const dom_osm_parser::get(const std::string & p_xml_string)
    {
      dom_parser l_dom_parser("osm");
      dom_osm_extractor<T> l_extractor;
      l_dom_parser.add_analyzer(l_extractor);
      l_dom_parser.parse_string(p_xml_string);
      return l_extractor.get_result();

    }

  //----------------------------------------------------------------------------
  const osm_api_capabilities * dom_osm_parser::get_capabilities(const std::string & p_xml_string)
    {
      dom_parser l_dom_parser("osm");
      dom_osm_api_capabilities_extractor l_extractor;
      l_dom_parser.add_analyzer(l_extractor);
      l_dom_parser.parse_string(p_xml_string);
      return l_extractor.get_result();
    }

  //----------------------------------------------------------------------------
  template <class T>
    const std::vector<T*> * const dom_osm_parser::get_list(const std::string & p_xml_string)
    {
      dom_parser l_dom_parser("osm");
      dom_osm_object_list_extractor<T> l_extractor;
      l_dom_parser.add_analyzer(l_extractor);
      l_dom_parser.parse_string(p_xml_string);
      return l_extractor.get_result();
    }
  
  //----------------------------------------------------------------------------
  const osm_api_data_types::osm_way * dom_osm_parser::get_full_way(const std::string & p_xml_string,
                                                                   std::vector<osm_api_data_types::osm_node*> & p_nodes)
    {
      dom_parser l_dom_parser("osm");
      dom_osm_full_way_extractor l_extractor(p_nodes);
      l_dom_parser.add_analyzer(l_extractor);
      l_dom_parser.parse_string(p_xml_string);
      return l_extractor.get_result();    
    }

  //----------------------------------------------------------------------------
  const osm_api_data_types::osm_relation * dom_osm_parser::get_full_relation(const std::string & p_xml_string,
                                                                             std::vector<osm_api_data_types::osm_node*> & p_nodes,
                                                                             std::vector<osm_api_data_types::osm_way*> & p_ways)
    {
      dom_parser l_dom_parser("osm");
      dom_osm_full_relation_extractor l_extractor(p_nodes,p_ways);
      l_dom_parser.add_analyzer(l_extractor);
      l_dom_parser.parse_string(p_xml_string);
      return l_extractor.get_result();    
    }

  //----------------------------------------------------------------------------
  const void dom_osm_parser::get(const std::string & p_xml_string,
                                 std::vector<osm_api_data_types::osm_node*> & p_nodes,
                                 std::vector<osm_api_data_types::osm_way*> & p_ways,
                                 std::vector<osm_api_data_types::osm_relation*> & p_relations)
  {
    dom_parser l_dom_parser("osm");
    dom_osm_full_extractor l_extractor(p_nodes,p_ways,p_relations);
    l_dom_parser.add_analyzer(l_extractor);
    l_dom_parser.parse_string(p_xml_string);
  }
}

#endif // _DOM_OSM_PARSER_H_
//EOF
