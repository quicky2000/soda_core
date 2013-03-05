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
#ifndef _OSM_CACHE_
#define _OSM_CACHE_

// Sql table types
#include "osm_cache_named_table.h"
#include "osm_cache_versionned_table.h"
#include "osm_cache_ordered_table.h"
#include "osm_cache_tag_table.h"
#include "information_table.h"

// Elements to be stored in table
#include "osm_cache_named_item.h"

#include "osm_cache_user.h"

#include "osm_cache_node.h"
#include "osm_cache_core_element.h"

#include "osm_cache_object_tag.h"
#include "osm_cache_way_member.h"
#include "osm_cache_relation_member.h"

// Tables descriptions
#include "osm_cache_named_item_table_description.h"
#include "osm_cache_user_table_description.h"
#include "osm_cache_node_table_description.h"

#include "osm_cache_core_element_table_description.h"

#include "osm_cache_object_tag_table_description.h"

#include "osm_cache_way_member_table_description.h"
#include "osm_cache_relation_member_table_description.h"

#include "quicky_exception.h"
#include <sstream>
#include <vector>

class sqlite3;
class sqlite3_stmt;


namespace osm_diff_watcher
{
  class soda_Ui_if;
  class osm_cache
  {
  public:
    osm_cache(soda_Ui_if & p_ui,const std::string & p_name = "osm_cache.sqlite3");
    void store(const osm_api_data_types::osm_object::t_osm_id & p_id,
               const std::string & p_user_name,
	       const osm_api_data_types::osm_object::t_osm_id & p_latest_changeset,
	       const std::string & p_date);
    void store(const osm_api_data_types::osm_node & p_node);
    void store(const osm_api_data_types::osm_way & p_way);
    void store(const osm_api_data_types::osm_relation & p_relation);
    const osm_api_data_types::osm_node * const load_node(const osm_api_data_types::osm_object::t_osm_id & p_id,
							 const osm_api_data_types::osm_core_element::t_osm_version & p_version);
    const osm_api_data_types::osm_way * const load_way(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                        const osm_api_data_types::osm_core_element::t_osm_version & p_version);
    const osm_api_data_types::osm_relation * const load_relation(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                             const osm_api_data_types::osm_core_element::t_osm_version & p_version);
    bool load_user(const osm_api_data_types::osm_object::t_osm_id & p_id,
                   std::string & p_name,
                   std::string & p_date);

    ~osm_cache(void);
  private:
    void store_tags(osm_cache_tag_table<osm_cache_object_tag> & p_table,
                    const osm_api_data_types::osm_core_element & p_element);

    void prepare_get_tags_statement(const std::string & p_table_name,sqlite3_stmt * & p_stmt_ptr);
    template <class T>
      void get_tags(sqlite3_stmt * p_stmt,
                    const osm_api_data_types::osm_object::t_osm_id & p_id,
                    const osm_api_data_types::osm_core_element::t_osm_version & p_version,
                    std::vector<std::pair<std::string,std::string> > & p_result);
    void get_relation_members(const osm_api_data_types::osm_object::t_osm_id & p_id,
			      const osm_api_data_types::osm_core_element::t_osm_version & p_version,
			      std::vector<osm_api_data_types::osm_relation_member *> & p_result);
    sqlite3 * m_db;
    sqlite3_stmt * m_get_node_tags_stmt;
    sqlite3_stmt * m_get_way_tags_stmt;
    sqlite3_stmt * m_get_relation_tags_stmt;
    sqlite3_stmt * m_get_relation_members_stmt;

    // Simple key tables
    osm_cache_named_table<osm_cache_named_item> m_tag_name_table;
    osm_cache_named_table<osm_cache_named_item> m_tag_value_table;
    osm_cache_named_table<osm_cache_named_item> m_relation_role_table;

    // Element tables
    osm_cache_named_table<osm_cache_user> m_user_table;

    osm_cache_versionned_table<osm_cache_node> m_node_table;
    osm_cache_versionned_table<osm_cache_core_element> m_way_table;
    osm_cache_versionned_table<osm_cache_core_element> m_relation_table;

    // Tag tables
    osm_cache_tag_table<osm_cache_object_tag> m_node_tag_table;
    osm_cache_tag_table<osm_cache_object_tag> m_way_tag_table;
    osm_cache_tag_table<osm_cache_object_tag> m_relation_tag_table;

    // member tables
    osm_cache_ordered_table<osm_cache_way_member> m_way_member_table;
    osm_cache_ordered_table<osm_cache_relation_member> m_relation_member_table;

    information_table m_informations;

    static const std::string m_schema_version;

  };

  //----------------------------------------------------------------------------
  template <class T>
    void osm_cache::get_tags(sqlite3_stmt * p_stmt,
                             const osm_api_data_types::osm_object::t_osm_id & p_id,
                             const osm_api_data_types::osm_core_element::t_osm_version & p_version,
                             std::vector<std::pair<std::string,std::string> > & p_result)
    {
  
      // Binding values to statement
      //----------------------------
      int l_status = sqlite3_bind_int64(p_stmt,sqlite3_bind_parameter_index(p_stmt,"$id"),p_id);
      if(l_status != SQLITE_OK)
        {
          std::stringstream l_stream;
          l_stream << "ERROR during binding of id parameter for get_by_id_version statement of " << osm_cache_base_table_description<T>::get_class_type() << " : " << sqlite3_errmsg(m_db) ;     
          throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
        }
    
      l_status = sqlite3_bind_int(p_stmt,sqlite3_bind_parameter_index(p_stmt,"$version"),p_version);
      if(l_status != SQLITE_OK)
        {
          std::stringstream l_stream;
          l_stream << "ERROR during binding of version parameter for get_by_id_version statement of " << osm_cache_base_table_description<T>::get_class_type() << " : " << sqlite3_errmsg(m_db) ;     
          throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
        }
    
      // Executing statement
      //---------------------
      while((l_status = sqlite3_step(p_stmt)) == SQLITE_ROW)
        {
          p_result.push_back(std::pair<std::string,std::string>((const char*)sqlite3_column_text(p_stmt,0),(const char*)sqlite3_column_text(p_stmt,1)));
        }
      if(l_status != SQLITE_DONE)
        {
          std::stringstream l_stream;
          l_stream << "ERROR during selection of tags : " << sqlite3_errmsg(m_db) ;
          throw quicky_exception::quicky_runtime_exception(l_stream.str(),__LINE__,__FILE__);
        }

      // Reset the statement for the next use
      //--------------------------------------
      l_status = sqlite3_reset(p_stmt);  
      if(l_status != SQLITE_OK)
        {
          std::stringstream l_stream;
          l_stream << "ERROR during reset of get_node_tags statement : " << sqlite3_errmsg(m_db) ;     
          throw quicky_exception::quicky_runtime_exception(l_stream.str(),__LINE__,__FILE__);
        }

      // Reset bindings because they are now useless
      //--------------------------------------------
      l_status = sqlite3_clear_bindings(p_stmt);
      if(l_status != SQLITE_OK)
        {
          std::stringstream l_stream;
          l_stream << "ERROR during reset of bindings of get_node_tags statement : " << sqlite3_errmsg(m_db) ;     
          throw quicky_exception::quicky_runtime_exception(l_stream.str(),__LINE__,__FILE__);
        }
        
    }
}
#endif // _OSM_CACHE_
