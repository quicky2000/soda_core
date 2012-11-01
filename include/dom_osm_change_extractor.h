#ifndef _DOM_OSM_CHANGE_EXTRACTOR_H_
#define _DOM_OSM_CHANGE_EXTRACTOR_H_

#include "dom_simple_analyzer_if.h"
#include "dom_generic_utilities.h"
#include <cstring>

namespace osm_diff_watcher
{
  class dom_osm_change_extractor: public dom_simple_analyzer_if
  {
  public:
    inline dom_osm_change_extractor(void);
    // Method inherited from dom_analyzer_if
    inline void analyze(const osm_diff_analyzer_dom_if::t_dom_tree & p_tree);
    inline void init(void){};
    // end of Method inherited from dom_analyzer_if
    inline std::vector<osm_api_data_types::osm_change*> * get_result(void)const;
 
  private:
    std::vector<osm_api_data_types::osm_change*> * m_result;
  };

  //----------------------------------------------------------------------------
  dom_osm_change_extractor::dom_osm_change_extractor(void):
    m_result(NULL)
    {
    }

    //----------------------------------------------------------------------------
    std::vector<osm_api_data_types::osm_change*> * dom_osm_change_extractor::get_result(void)const
      {
        return m_result;
      }

    //----------------------------------------------------------------------------
    void dom_osm_change_extractor::analyze(const osm_diff_analyzer_dom_if::t_dom_tree & p_tree)
    {
      assert(!strcmp(p_tree.getName(),"osmChange"));
      int l_nb_child_node = p_tree.nChildNode();
      std::cout << "Nb child " << l_nb_child_node << std::endl ;
      
      m_result = new std::vector<osm_api_data_types::osm_change*>;
      for(int l_index = 0 ; l_index < l_nb_child_node ; ++l_index)
        {
          const osm_diff_analyzer_dom_if::t_dom_tree & l_node = p_tree.getChildNode(l_index);
          osm_api_data_types::osm_change::t_osm_change_type l_change_type = osm_api_data_types::osm_change::get_change_type(l_node.getName());
          int l_nb_child_object_nodes = l_node.nChildNode();
          for(int l_index_object = 0 ; l_index_object < l_nb_child_object_nodes ; ++l_index_object)
            {
              const osm_diff_analyzer_dom_if::t_dom_tree & l_object_node = l_node.getChildNode(l_index_object);
              osm_api_data_types::osm_core_element::t_osm_type l_osm_type = osm_api_data_types::osm_core_element::get_osm_type(l_object_node.getName());
              switch(l_osm_type)
                {
                case osm_api_data_types::osm_core_element::NODE :
                  m_result->push_back(new osm_api_data_types::osm_change_generic<osm_api_data_types::osm_node>(l_change_type,generic_dom_utilities<XMLNode>::extract_info<osm_api_data_types::osm_node>(l_object_node,false,l_change_type == osm_api_data_types::osm_change::DELETION)));
                  break;
                case osm_api_data_types::osm_core_element::WAY :
                  m_result->push_back(new osm_api_data_types::osm_change_generic<osm_api_data_types::osm_way>(l_change_type,generic_dom_utilities<XMLNode>::extract_info<osm_api_data_types::osm_way>(l_object_node)));
                  break;
                case osm_api_data_types::osm_core_element::RELATION :
                  m_result->push_back(new osm_api_data_types::osm_change_generic<osm_api_data_types::osm_relation>(l_change_type,generic_dom_utilities<XMLNode>::extract_info<osm_api_data_types::osm_relation>(l_object_node)));
                  break;
                default:
                  exit(-1);
                  break;
                }
            }
        }
    }

}
#endif // _DOM_OSM_CHANGE_EXTRACTOR_H_
//EOF
