#include "common_api_wrapper.h"

#include <cassert>

namespace osm_diff_watcher
{
  //----------------------------------------------------------------------------
  void common_api_wrapper::register_function(void ** p_api_ptr,uint32_t p_size)
  {
    assert(p_size == COMMON_API_IF_SIZE);
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_API_VERSION] = (void*) common_api_wrapper::get_api_version;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_USER_SUBSCRIPTION_DATE] = (void*) common_api_wrapper::get_user_subscription_date;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_NODE] = (void*) common_api_wrapper::get_node;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_NODE_VERSION] = (void*) common_api_wrapper::get_node_version;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_NODE_HISTORY] = (void*) common_api_wrapper::get_node_history;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_NODE_WAYS] = (void*) common_api_wrapper::get_node_ways;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_NODE_RELATIONS] = (void*) common_api_wrapper::get_node_relations;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_NODES] = (void*) common_api_wrapper::get_nodes;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_WAY] = (void*) common_api_wrapper::get_way;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_WAY_VERSION] = (void*) common_api_wrapper::get_way_version;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_WAY_HISTORY] = (void*) common_api_wrapper::get_way_history;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_WAY_RELATIONS] = (void*) common_api_wrapper::get_way_relations;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_WAY_FULL] = (void*) common_api_wrapper::get_way_full;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_WAYS] = (void*) common_api_wrapper::get_ways;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_RELATION] = (void*) common_api_wrapper::get_relation;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_RELATION_VERSION] = (void*) common_api_wrapper::get_relation_version;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_RELATION_HISTORY] = (void*) common_api_wrapper::get_relation_history;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_RELATION_RELATIONS] = (void*) common_api_wrapper::get_relation_relations;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_RELATION_FULL] = (void*) common_api_wrapper::get_relation_full;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_RELATIONS] = (void*) common_api_wrapper::get_relations;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_CHANGESET] = (void*) common_api_wrapper::get_changeset;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_CHANGESET_CONTENT] = (void*) common_api_wrapper::get_changeset_content;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_CHANGESETS] = (void*) common_api_wrapper::get_changesets;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_MAP] = (void*) common_api_wrapper::get_map;
  }

  //----------------------------------------------------------------------------
  const char *common_api_wrapper::get_api_version(void)
  {
    return COMMON_API_IF_VERSION;
  }

  osm_ressources common_api_wrapper::m_ressources;
  osm_api common_api_wrapper::m_api;
}
//EOF
