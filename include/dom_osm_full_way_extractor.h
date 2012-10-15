#ifndef _DOM_OSM_FULL_WAY_EXTRACTOR_H_
#define _DOM_OSM_FULL_WAY_EXTRACTOR_H_

#include "dom_analyser_if.h"
#include "dom_generic_utilities.h"
#include <cstring>

namespace osm_diff_watcher
{
  class dom_osm_full_way_extractor: public dom_analyser_if
  {
  public:
    inline dom_osm_full_way_extractor(std::vector<osm_node*> & p_nodes);
    // Method inherited from dom_analyser_if
    inline void analyse(const t_dom_tree & p_tree);
    // end of Method inherited from dom_analyser_if
    inline osm_way * get_result(void);
  private:
    std::vector<osm_node*> & m_nodes;
    osm_way * m_result;
   
  };

  //----------------------------------------------------------------------------
  osm_way * dom_osm_full_way_extractor::get_result(void)
    {
      return m_result;
    }

  //----------------------------------------------------------------------------
  dom_osm_full_way_extractor::dom_osm_full_way_extractor(std::vector<osm_node*> & p_nodes):
    m_nodes(p_nodes),
    m_result(NULL)
  {
  }

  //----------------------------------------------------------------------------
  void dom_osm_full_way_extractor::analyse(const t_dom_tree & p_tree)
  {
      assert(!strcmp("osm",p_tree.getName()));
      int l_nb_version = p_tree.nChildNode();
      for(int l_index = 0; l_index < l_nb_version ; ++l_index)
	{
	  t_dom_tree l_node = p_tree.getChildNode(l_index);
	  if(osm_node::get_type_str() == l_node.getName())
	    {
	      m_nodes.push_back(generic_dom_utilities<t_dom_tree>::extract_info<osm_node>(l_node,true));
	    }
	  else if(osm_way::get_type_str() == l_node.getName())
	    {
	      m_result = generic_dom_utilities<t_dom_tree>::extract_info<osm_way>(l_node,true);
	    }
	  else
	    {
	      std::cout << "Unexpected node type \"" << l_node.getName() << "\"" << std::endl ;
	      exit(-1);
	    }
	}
  }
}
#endif // _DOM_OSM_FULL_WAY_EXTRACTOR_H_
//EOF