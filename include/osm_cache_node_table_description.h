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
#ifndef _OSM_CACHE_NODE_TABLE_DESCRIPTION_H_
#define _OSM_CACHE_NODE_TABLE_DESCRIPTION_H_

#include "osm_cache_base_table_description.h"
#include "osm_cache_core_element_table_description.h"
#include "osm_cache_node.h"
#include <sqlite3.h>
#include <iostream>
#include <cstdlib>

namespace osm_diff_watcher
{
  template <> 
    class osm_cache_base_table_description<osm_cache_node>
    {

    public:
      inline static const std::string & get_class_type(void);
      inline static const std::string get_table_fields_declaration(void);
      inline static const std::string get_table_fields(void);
      inline static const std::string get_update_fields(void);
      inline static const std::string get_field_values(void);
      inline static void bind_item_values(const osm_cache_node & p_name,sqlite3_stmt* p_stmt,sqlite3 *p_db);
      inline static osm_cache_node get_item_from_row(sqlite3_stmt* p_stmt);
    private:
      static const std::string m_class_type;
      static const std::string m_table_fields_declaration;
      static const std::string m_table_fields;
      static const std::string m_update_fields;
      static const std::string m_field_values;
    };


  //------------------------------------------------------------------------------
  const std::string & osm_cache_base_table_description<osm_cache_node>::get_class_type(void)
    {
      return m_class_type;
    }

  //------------------------------------------------------------------------------
  const std::string osm_cache_base_table_description<osm_cache_node>::get_table_fields_declaration(void)
    {
      return osm_cache_base_table_description<osm_cache_core_element>::get_table_fields_declaration() + ", " + "Lat REAL,Lon Real";
    }

  //------------------------------------------------------------------------------
  const std::string osm_cache_base_table_description<osm_cache_node>::get_table_fields(void)
    {
      return osm_cache_base_table_description<osm_cache_core_element>::get_table_fields() + ", " + "Lat, Lon";
    }

  //------------------------------------------------------------------------------
  const std::string osm_cache_base_table_description<osm_cache_node>::get_update_fields(void)
    {
      return osm_cache_base_table_description<osm_cache_core_element>::get_update_fields() + ", " + "Lat = $lat, Lon = $lon";
    }

  //------------------------------------------------------------------------------
  const std::string osm_cache_base_table_description<osm_cache_node>::get_field_values(void)
    {
      return osm_cache_base_table_description<osm_cache_core_element>::get_field_values() + ", " + "$lat , $lon";
    }

  //------------------------------------------------------------------------------
  void osm_cache_base_table_description<osm_cache_node>::bind_item_values(const osm_cache_node & p_node,sqlite3_stmt* p_stmt,sqlite3 *p_db)
  {
    osm_cache_base_table_description<osm_cache_core_element>::bind_item_values(p_node,p_stmt,p_db);
    int l_status = sqlite3_bind_double(p_stmt,sqlite3_bind_parameter_index(p_stmt,"$lat"),p_node.get_lat());
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during binding of lat parameter for update statement of " << get_class_type() << " : " << sqlite3_errmsg(p_db) << std::endl ;     
	exit(-1);
      }  
    l_status = sqlite3_bind_double(p_stmt,sqlite3_bind_parameter_index(p_stmt,"$lon"),p_node.get_lon());
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during binding of lon parameter for update statement of " << get_class_type() << " : " << sqlite3_errmsg(p_db) << std::endl ;     
	exit(-1);
      }  
  }

  //------------------------------------------------------------------------------
  osm_cache_node osm_cache_base_table_description<osm_cache_node>::get_item_from_row(sqlite3_stmt* p_stmt)
    {
      return osm_cache_node(osm_cache_base_table_description<osm_cache_core_element>::get_item_from_row(p_stmt),
                            sqlite3_column_double(p_stmt,5),// Lat
                            sqlite3_column_double(p_stmt,6)// Lon
                            );
    }
}
#endif // _OSM_CACHE_NODE_TABLE_DESCRIPTION_H_
//EOF
