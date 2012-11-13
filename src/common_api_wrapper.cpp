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
#include "common_api_wrapper.h"

#include <cassert>

namespace osm_diff_watcher
{
  //----------------------------------------------------------------------------
  void common_api_wrapper::register_function(uintptr_t * p_api_ptr,uint32_t p_size)
  {
    assert(p_size == COMMON_API_IF_SIZE);
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_API_VERSION] = (uintptr_t) common_api_wrapper::get_api_version;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_USER_SUBSCRIPTION_DATE] = (uintptr_t) common_api_wrapper::get_user_subscription_date;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_NODE] = (uintptr_t) common_api_wrapper::get_node;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_NODE_VERSION] = (uintptr_t) common_api_wrapper::get_node_version;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_NODE_HISTORY] = (uintptr_t) common_api_wrapper::get_node_history;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_NODE_WAYS] = (uintptr_t) common_api_wrapper::get_node_ways;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_NODE_RELATIONS] = (uintptr_t) common_api_wrapper::get_node_relations;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_NODES] = (uintptr_t) common_api_wrapper::get_nodes;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_WAY] = (uintptr_t) common_api_wrapper::get_way;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_WAY_VERSION] = (uintptr_t) common_api_wrapper::get_way_version;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_WAY_HISTORY] = (uintptr_t) common_api_wrapper::get_way_history;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_WAY_RELATIONS] = (uintptr_t) common_api_wrapper::get_way_relations;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_WAY_FULL] = (uintptr_t) common_api_wrapper::get_way_full;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_WAYS] = (uintptr_t) common_api_wrapper::get_ways;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_RELATION] = (uintptr_t) common_api_wrapper::get_relation;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_RELATION_VERSION] = (uintptr_t) common_api_wrapper::get_relation_version;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_RELATION_HISTORY] = (uintptr_t) common_api_wrapper::get_relation_history;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_RELATION_RELATIONS] = (uintptr_t) common_api_wrapper::get_relation_relations;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_RELATION_FULL] = (uintptr_t) common_api_wrapper::get_relation_full;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_RELATIONS] = (uintptr_t) common_api_wrapper::get_relations;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_CHANGESET] = (uintptr_t) common_api_wrapper::get_changeset;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_CHANGESET_CONTENT] = (uintptr_t) common_api_wrapper::get_changeset_content;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_CHANGESETS] = (uintptr_t) common_api_wrapper::get_changesets;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_MAP] = (uintptr_t) common_api_wrapper::get_map;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::CACHE_NODE] = (uintptr_t) common_api_wrapper::cache_node;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::CACHE_WAY] = (uintptr_t) common_api_wrapper::cache_way;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::CACHE_RELATION] = (uintptr_t) common_api_wrapper::cache_relation;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::CACHE_USER] = (uintptr_t) common_api_wrapper::cache_user;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_OSM_CHANGE_FILE_CONTENT] = (uintptr_t) common_api_wrapper::get_osm_change_file_content;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_OSM_FILE_CONTENT] = (uintptr_t) common_api_wrapper::get_osm_file_content;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_USER_BROWSE_URL] = (uintptr_t) common_api_wrapper::get_user_browse_url;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_OBJECT_BROWSE_URL] = (uintptr_t) common_api_wrapper::get_object_browse_url;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_API_OBJECT_URL] = (uintptr_t) common_api_wrapper::get_api_object_url;
  }

  //----------------------------------------------------------------------------
  const char *common_api_wrapper::get_api_version(void)
  {
    return COMMON_API_IF_VERSION;
  }

  //----------------------------------------------------------------------------
  common_api_wrapper::common_api_wrapper(osm_ressources & p_ressources):
    m_ressources(p_ressources),
    m_api(p_ressources)
  {
  }

  //----------------------------------------------------------------------------
  common_api_wrapper & common_api_wrapper::instance(osm_ressources & p_ressources)
  {
    if(m_instance == NULL)
      {
        m_instance = new common_api_wrapper(p_ressources);
      }
    return *m_instance;
  }
   //----------------------------------------------------------------------------
  void common_api_wrapper::remove_instance(void)
  {
    delete m_instance;
  }

  common_api_wrapper * common_api_wrapper::m_instance = NULL;
}
//EOF
