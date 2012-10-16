#include "dom2cpp_analyser.h"
#include "osm_change.h"
#include "osm_node.h"
#include "dom_generic_utilities.h"
#include <cassert>
#include <iostream>
#include <string.h>

namespace osm_diff_watcher
{
  //------------------------------------------------------------------------------
  dom2cpp_analyzer::dom2cpp_analyzer(const std::string & p_name):
    m_name(p_name)
  {
  }

  //------------------------------------------------------------------------------
  void dom2cpp_analyzer::analyze(const osm_diff_analyzer_if::t_dom_tree & p_tree)
  {
    assert(!strcmp(p_tree.getName(),"osmChange"));
    int l_nb_child_node = p_tree.nChildNode();
    std::cout << "Nb child " << l_nb_child_node << std::endl ;
    std::vector<osm_change*> l_changes;

    for(int l_index = 0 ; l_index < l_nb_child_node ; ++l_index)
      {
	const osm_diff_analyzer_if::t_dom_tree & l_node = p_tree.getChildNode(l_index);
	osm_change::t_osm_change_type l_change_type = osm_change::get_change_type(l_node.getName());
	int l_nb_child_object_nodes = l_node.nChildNode();
	for(int l_index_object = 0 ; l_index_object < l_nb_child_object_nodes ; ++l_index_object)
	  {
	    const osm_diff_analyzer_if::t_dom_tree & l_object_node = l_node.getChildNode(l_index_object);
	    osm_core_element::t_osm_type l_osm_type = osm_core_element::get_osm_type(l_object_node.getName());
	    switch(l_osm_type)
	      {
	      case osm_core_element::NODE :
		l_changes.push_back(new osm_change_generic<osm_node>(l_change_type,generic_dom_utilities<XMLNode>::extract_info<osm_node>(l_object_node,false,l_change_type == osm_change::DELETION)));
		break;
	      case osm_core_element::WAY :
                l_changes.push_back(new osm_change_generic<osm_way>(l_change_type,generic_dom_utilities<XMLNode>::extract_info<osm_way>(l_object_node)));
		break;
	      case osm_core_element::RELATION :
                l_changes.push_back(new osm_change_generic<osm_relation>(l_change_type,generic_dom_utilities<XMLNode>::extract_info<osm_relation>(l_object_node)));
		break;
	      default:
		exit(-1);
		break;
	      }
	  }
      }
    

    // Delete changes
    std::vector<osm_change*>::iterator l_iter = l_changes.begin();
    std::vector<osm_change*>::iterator l_iter_end = l_changes.end();
    while(l_iter != l_iter_end)
      {
        delete *l_iter;
        ++l_iter;
      }
  }
  const std::string dom2cpp_analyzer::m_input_type = "dom";
  const std::string dom2cpp_analyzer::m_output_type = "osm_cpp";
  const std::string dom2cpp_analyzer::m_type = "dom2cpp_analyzer";
}
//EOF
