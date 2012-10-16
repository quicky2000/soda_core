#include "osm_diff_watcher.h"
#include <iostream>
#include <stdlib.h>

#include "osm_api.h"
#include "osm_node.h"

int main(int p_argc,char ** p_argv)
{
  std::cout << "Nb arguments : " << p_argc << std::endl ;
  uint64_t l_start_seq_number = 0;
  if(p_argc == 2)
    {
      l_start_seq_number = atoll(p_argv[1]);
    }

  osm_diff_watcher::osm_api l_api;
#ifdef TEST_API
  // Get API capabilities
  const osm_diff_watcher::osm_api_capabilities * l_capabilities = l_api.get_capabilities();
  assert(l_capabilities);
  std::cout << * l_capabilities << std::endl ;

  //Get map elements
  std::vector<osm_diff_watcher::osm_node*> l_map_nodes;
  std::vector<osm_diff_watcher::osm_way*> l_map_ways;
  std::vector<osm_diff_watcher::osm_relation*> l_map_relations;
  l_api.get_map(osm_diff_watcher::osm_bounding_box(5.7076801,45.1925595,5.7112314,45.2000492),l_map_nodes,l_map_ways,l_map_relations);
  for(std::vector<osm_diff_watcher::osm_node*>::const_iterator l_iter = l_map_nodes.begin();
      l_iter != l_map_nodes.end();
      ++l_iter)
    {
      std::cout << **l_iter;
    }
  for(std::vector<osm_diff_watcher::osm_way*>::const_iterator l_iter = l_map_ways.begin();
      l_iter != l_map_ways.end();
      ++l_iter)
    {
      std::cout << **l_iter;
    }
  for(std::vector<osm_diff_watcher::osm_relation*>::const_iterator l_iter = l_map_relations.begin();
      l_iter != l_map_relations.end();
      ++l_iter)
    {
      std::cout << **l_iter;
    }
  exit(-1);

  // Get changeset of quicky
  const std::vector<osm_diff_watcher::osm_changeset*> * l_list_changeset = l_api.get_changesets(osm_diff_watcher::osm_bounding_box(),0,"quicky","2012-09-07T14:02:41Z","",false,false);
  for(std::vector<osm_diff_watcher::osm_changeset*>::const_iterator l_iter = l_list_changeset->begin();
      l_iter != l_list_changeset->end();
      ++l_iter)
    {
      std::cout << **l_iter;
    }
  exit(-1);

  // Get changeset content
  const std::vector<osm_diff_watcher::osm_change *> * l_changes = l_api.get_changeset_content(13369487);
  for(std::vector<osm_diff_watcher::osm_change*>::const_iterator l_iter = l_changes->begin();
      l_iter != l_changes->end();
      ++l_iter)
    {
      std::cout << **l_iter;
    }
  exit(-1);

  // Get changeset
  const osm_diff_watcher::osm_changeset * l_changeset = l_api.get<osm_diff_watcher::osm_changeset>(13369487);
  assert(l_changeset);
  std::cout << *l_changeset;

  // Get Node
  const osm_diff_watcher::osm_node * l_node = l_api.get<osm_diff_watcher::osm_node>(474827214);
  assert(l_node);
  std::cout << *l_node ; 

  // Get way
  const osm_diff_watcher::osm_way * l_way = l_api.get<osm_diff_watcher::osm_way>(173118043);
  assert(l_way);
  std::cout << *l_way ; 

  // Get full way
  std::cout << "--------------------------------" << std::endl ;
  std::vector<osm_diff_watcher::osm_node*> l_way_nodes;
  const osm_diff_watcher::osm_way * l_full_way = l_api.get_full_way(173118043,l_way_nodes);
  assert(l_full_way);
  std::cout << *l_full_way ; 
  for(std::vector<osm_diff_watcher::osm_node*>::const_iterator l_iter = l_way_nodes.begin();
      l_iter != l_way_nodes.end();
      ++l_iter)
    {
      std::cout << **l_iter;
    }
  std::cout << "--------------------------------" << std::endl ;

  // Get relation for bus line 30
  const osm_diff_watcher::osm_relation * l_relation = l_api.get<osm_diff_watcher::osm_relation>(216546);
  assert(l_relation);
  std::cout << * l_relation << std::endl ;

  // Get full relation for bus line 30
  std::cout << "--------------------------------" << std::endl ;
  std::vector<osm_diff_watcher::osm_node*> l_relation_nodes;
  std::vector<osm_diff_watcher::osm_way*> l_relation_ways;
  const osm_diff_watcher::osm_relation * l_full_relation = l_api.get_full_relation(216546,l_relation_nodes,l_relation_ways);
  assert(l_relation);
  std::cout << * l_full_relation << std::endl ;
  for(std::vector<osm_diff_watcher::osm_node*>::const_iterator l_iter = l_relation_nodes.begin();
      l_iter != l_relation_nodes.end();
      ++l_iter)
    {
      std::cout << **l_iter;
    }
  for(std::vector<osm_diff_watcher::osm_way*>::const_iterator l_iter = l_relation_ways.begin();
      l_iter != l_relation_ways.end();
      ++l_iter)
    {
      std::cout << **l_iter;
    }
  std::cout << "--------------------------------" << std::endl ;

  // Get list of nodes
  std::vector<osm_diff_watcher::osm_object::t_osm_id> l_node_ids;
  l_node_ids.push_back(344567058);
  l_node_ids.push_back(474827214);
  const std::vector<osm_diff_watcher::osm_node*> * l_list_node = l_api.get<osm_diff_watcher::osm_node>(l_node_ids);
  for(std::vector<osm_diff_watcher::osm_node*>::const_iterator l_iter = l_list_node->begin();
      l_iter != l_list_node->end();
      ++l_iter)
    {
      std::cout << **l_iter;
    }

  // Get list of ways
  std::vector<osm_diff_watcher::osm_object::t_osm_id> l_way_ids;
  l_way_ids.push_back(173118043);
  l_way_ids.push_back(184257352);
  const std::vector<osm_diff_watcher::osm_way*> * l_list_way = l_api.get<osm_diff_watcher::osm_way>(l_way_ids);
  for(std::vector<osm_diff_watcher::osm_way*>::const_iterator l_iter = l_list_way->begin();
      l_iter != l_list_way->end();
      ++l_iter)
    {
      std::cout << **l_iter;
    }

  // Get list of relations
  std::vector<osm_diff_watcher::osm_object::t_osm_id> l_relation_ids;
  l_relation_ids.push_back(216546);
  l_relation_ids.push_back(191189);
  // TO COMPLETE  l_relation_ids.push_back();
  const std::vector<osm_diff_watcher::osm_relation*> * l_list_relation = l_api.get<osm_diff_watcher::osm_relation>(l_relation_ids);
  for(std::vector<osm_diff_watcher::osm_relation*>::const_iterator l_iter = l_list_relation->begin();
      l_iter != l_list_relation->end();
      ++l_iter)
    {
      std::cout << **l_iter;
    }

  // Get history for a node
  const std::vector<osm_diff_watcher::osm_node *> * const l_node_history = l_api.get_history<osm_diff_watcher::osm_node>(344567058);
  for(std::vector<osm_diff_watcher::osm_node *>::const_iterator l_iter = l_node_history->begin();
      l_iter != l_node_history->end();
      ++l_iter)
    {
      std::cout << "-------------------------------" << std::endl ;
      std::cout << *(*l_iter) << std::endl ;
    }

  // Get all ways for a node shared by Rue des martyrs and Rampe provisoire
  const std::vector<osm_diff_watcher::osm_way *> * const l_ways = l_api.get<osm_diff_watcher::osm_node,osm_diff_watcher::osm_way>(1947164927);
  for(std::vector<osm_diff_watcher::osm_way *>::const_iterator l_iter = l_ways->begin();
      l_iter != l_ways->end();
      ++l_iter)
    {
      std::cout << "-------------------------------" << std::endl ;
      std::cout << *(*l_iter) << std::endl ;
    }

  // Get all relations using way Rampe Provisoire
  const std::vector<osm_diff_watcher::osm_relation *> * const l_way_relations = l_api.get<osm_diff_watcher::osm_way,osm_diff_watcher::osm_relation>(184257352);
  for(std::vector<osm_diff_watcher::osm_relation *>::const_iterator l_iter = l_way_relations->begin();
      l_iter != l_way_relations->end();
      ++l_iter)
    {
      std::cout << "-------------------------------" << std::endl ;
      std::cout << *(*l_iter) << std::endl ;
    }
  
  // Get all relations using bus stop victor hugo
  const std::vector<osm_diff_watcher::osm_relation *> * const l_node_relations = l_api.get<osm_diff_watcher::osm_node,osm_diff_watcher::osm_relation>(1829038636);
  for(std::vector<osm_diff_watcher::osm_relation *>::const_iterator l_iter = l_node_relations->begin();
      l_iter != l_node_relations->end();
      ++l_iter)
    {
      std::cout << "-------------------------------" << std::endl ;
      std::cout << *(*l_iter) << std::endl ;
    }
  
  exit(-1);
#endif

  osm_diff_watcher::osm_diff_watcher l_watcher;
  l_watcher.run(l_start_seq_number);
}
//EOF
