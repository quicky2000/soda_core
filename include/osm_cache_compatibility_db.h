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
#ifndef _OSM_CACHE_COMPATIBILITY_DB_
#define _OSM_CACHE_COMPATIBILITY_DB_

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

class sqlite3;


namespace osm_diff_watcher
{
  class osm_cache_compatibility_db
  {
  public:
    osm_cache_compatibility_db(const std::string & p_name,const std::string & p_current_version);
    ~osm_cache_compatibility_db(void);
  private:
    void upgrade(const std::string & p_from_version);
    void upgrade_from_0_1(void);

    sqlite3 * m_db;

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
  };

}
#endif // _OSM_CACHE_COMPATIBILITY_DB_
//EOF
