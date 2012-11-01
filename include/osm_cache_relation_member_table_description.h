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
#ifndef _OSM_CACHE_RELATION_MEMBER_TABLE_DESCRIPTION_H_
#define _OSM_CACHE_RELATION_MEMBER_TABLE_DESCRIPTION_H_

#include "osm_cache_base_table_description.h"
#include "osm_cache_way_member_table_description.h"
#include "osm_cache_relation_member.h"
#include <sqlite3.h>
#include <iostream>
#include <cstdlib>

namespace osm_diff_watcher
{
  template <> 
    class osm_cache_base_table_description<osm_cache_relation_member>
    {

    public:
      inline static const std::string & get_class_type(void);
      inline static const std::string get_table_fields_declaration(void);
      inline static const std::string get_table_fields(void);
      inline static const std::string get_update_fields(void);
      inline static const std::string get_field_values(void);
      inline static void bind_item_values(const osm_cache_relation_member & p_name,sqlite3_stmt* p_stmt,sqlite3 *p_db);
      inline static osm_cache_relation_member get_item_from_row(sqlite3_stmt* p_stmt);
    private:
      static const std::string m_class_type;
      static const std::string m_table_fields_declaration;
      static const std::string m_table_fields;
      static const std::string m_update_fields;
      static const std::string m_field_values;
    };


  //------------------------------------------------------------------------------
  const std::string & osm_cache_base_table_description<osm_cache_relation_member>::get_class_type(void)
    {
      return m_class_type;
    }

  //------------------------------------------------------------------------------
  const std::string osm_cache_base_table_description<osm_cache_relation_member>::get_table_fields_declaration(void)
    {
      return osm_cache_base_table_description<osm_cache_way_member>::get_table_fields_declaration() + ", " + "Osm_type INTEGER, Role_id INTEGER";
    }

  //------------------------------------------------------------------------------
  const std::string osm_cache_base_table_description<osm_cache_relation_member>::get_table_fields(void)
    {
      return osm_cache_base_table_description<osm_cache_way_member>::get_table_fields() + ", " + "Osm_type, Role_id";
    }

  //------------------------------------------------------------------------------
  const std::string osm_cache_base_table_description<osm_cache_relation_member>::get_update_fields(void)
    {
      return osm_cache_base_table_description<osm_cache_way_member>::get_update_fields() + ", " + "Osm_type = $osm_type ,Role_id = $role_id";
    }

  //------------------------------------------------------------------------------
  const std::string osm_cache_base_table_description<osm_cache_relation_member>::get_field_values(void)
    {
      return osm_cache_base_table_description<osm_cache_way_member>::get_field_values() + ", " + "$osm_type, $role_id";
    }


  //------------------------------------------------------------------------------
  void osm_cache_base_table_description<osm_cache_relation_member>::bind_item_values(const osm_cache_relation_member & p_relation_member,sqlite3_stmt* p_stmt,sqlite3 *p_db)
  {
    osm_cache_base_table_description<osm_cache_way_member>::bind_item_values(p_relation_member,p_stmt,p_db);

    int l_status = sqlite3_bind_int(p_stmt,sqlite3_bind_parameter_index(p_stmt,"$osm_type"),p_relation_member.get_osm_type());
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during binding of osm_type parameter for update statement of " << get_class_type() << " : " << sqlite3_errmsg(p_db) << std::endl ;     
	exit(-1);
      }  
   l_status = sqlite3_bind_int64(p_stmt,sqlite3_bind_parameter_index(p_stmt,"$role_id"),p_relation_member.get_role_id());
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during binding of role_id parameter for update statement of " << get_class_type() << " : " << sqlite3_errmsg(p_db) << std::endl ;     
	exit(-1);
      }  
  }

  //------------------------------------------------------------------------------
  osm_cache_relation_member osm_cache_base_table_description<osm_cache_relation_member>::get_item_from_row(sqlite3_stmt* p_stmt)
  {
    return osm_cache_relation_member(osm_cache_base_table_description<osm_cache_way_member>::get_item_from_row(p_stmt),
                                     (osm_api_data_types::osm_core_element::t_osm_type)sqlite3_column_int(p_stmt,4),//osm_type
                                     sqlite3_column_int64(p_stmt,5)// Role_Id
                                  );
  }
}
#endif // _OSM_CACHE_RELATION_MEMBER_TABLE_DESCRIPTION_H_
//EOF
