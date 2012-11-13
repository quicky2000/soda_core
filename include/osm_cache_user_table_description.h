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
#ifndef _OSM_CACHE_USER_TABLE_DESCRIPTION_H_
#define _OSM_CACHE_USER_TABLE_DESCRIPTION_H_

#include "osm_cache_base_table_description.h"
#include "osm_cache_user.h"
#include <sqlite3.h>
#include <iostream>
#include <cstdlib>

namespace osm_diff_watcher
{
  template <> 
    class osm_cache_base_table_description<osm_cache_user>
    {

    public:
      inline static const std::string & get_class_type(void);
      inline static const std::string & get_table_fields_declaration(void);
      inline static const std::string & get_table_fields(void);
      inline static const std::string & get_update_fields(void);
      inline static const std::string & get_field_values(void);
      inline static void bind_item_values(const osm_cache_user & p_name,sqlite3_stmt* p_stmt,sqlite3 *p_db);
      inline static osm_cache_user get_item_from_row(sqlite3_stmt* p_stmt);
    private:
      static const std::string m_class_type;
      static const std::string m_table_fields_declaration;
      static const std::string m_table_fields;
      static const std::string m_update_fields;
      static const std::string m_field_values;
    };


  //------------------------------------------------------------------------------
  const std::string & osm_cache_base_table_description<osm_cache_user>::get_class_type(void)
    {
      return m_class_type;
    }

  //------------------------------------------------------------------------------
  const std::string & osm_cache_base_table_description<osm_cache_user>::get_table_fields_declaration(void)
    {
      return m_table_fields_declaration;
    }

  //------------------------------------------------------------------------------
  const std::string & osm_cache_base_table_description<osm_cache_user>::get_table_fields(void)
    {
      return m_table_fields;
    }

  //------------------------------------------------------------------------------
  const std::string & osm_cache_base_table_description<osm_cache_user>::get_update_fields(void)
    {
      return m_update_fields;
    }

  //------------------------------------------------------------------------------
  const std::string & osm_cache_base_table_description<osm_cache_user>::get_field_values(void)
    {
      return m_field_values;
    }

  //------------------------------------------------------------------------------
  void osm_cache_base_table_description<osm_cache_user>::bind_item_values(const osm_cache_user & p_user,sqlite3_stmt* p_stmt,sqlite3 *p_db)
  {
    int l_status = sqlite3_bind_text(p_stmt,sqlite3_bind_parameter_index(p_stmt,"$name"),p_user.get_name().c_str(),-1,SQLITE_STATIC);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during binding of name parameter for update statement of " << get_class_type() << " : " << sqlite3_errmsg(p_db) << std::endl ;     
	exit(-1);
      }  
  l_status = sqlite3_bind_text(p_stmt,sqlite3_bind_parameter_index(p_stmt,"$date"),p_user.get_date().c_str(),-1,SQLITE_STATIC);
  if(l_status != SQLITE_OK)
    {
      std::cout << "ERROR during binding of date parameter for update statement of " << get_class_type() << " : " << sqlite3_errmsg(p_db) << std::endl ;     
      exit(-1);
    }  
  l_status = sqlite3_bind_int64(p_stmt,sqlite3_bind_parameter_index(p_stmt,"$latest_changeset"),p_user.get_latest_changeset());
  if(l_status != SQLITE_OK)
    {
      std::cout << "ERROR during binding of latest_changeset parameter for update statement of " << get_class_type() << " : " << sqlite3_errmsg(p_db) << std::endl ;     
      exit(-1);
    }  

  }

  //------------------------------------------------------------------------------
  osm_cache_user osm_cache_base_table_description<osm_cache_user>::get_item_from_row(sqlite3_stmt* p_stmt)
    {
      return osm_cache_user(sqlite3_column_int64(p_stmt,0),// Id
                            (const char*)sqlite3_column_text(p_stmt,1), // Name
                            (const char*)sqlite3_column_text(p_stmt,2), //Data
			    sqlite3_column_int64(p_stmt,3)
                            );
    }
}
#endif // _OSM_CACHE_USER_TABLE_DESCRIPTION_H_
//EOF
