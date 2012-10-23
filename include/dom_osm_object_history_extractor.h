#ifndef _DOM_OSM_OBJECT_HISTORY_EXTRACTOR_H_
#define _DOM_OSM_OBJECT_HISTORY_EXTRACTOR_H_

#include "dom_extractor_base.h"
#include "dom_generic_utilities.h"
#include <cstring>

namespace osm_diff_watcher
{
  template <class T>
  class dom_osm_object_history_extractor: public dom_extractor_base
  {
  public:
    dom_osm_object_history_extractor(void);
    // Method inherited from dom_analyzer_if
    void analyze(const osm_diff_analyzer_if::t_dom_tree & p_tree);
    // end of Method inherited from dom_analyzer_if
    std::vector<T*> * get_result(void)const;
  private:
    std::vector<T*> * m_result;
  }
  ;

  //----------------------------------------------------------------------------
  template <class T>
    dom_osm_object_history_extractor<T>::dom_osm_object_history_extractor(void):
    dom_extractor_base("dom_osm_object_history_extractor<"+t::get_type_str()+">"),
    m_result(NULL)
    {
    }

  //----------------------------------------------------------------------------
  template <class T>
    std::vector<T*> * dom_osm_object_history_extractor<T>::get_result(void)const
    {
      return m_result;
    }

  //----------------------------------------------------------------------------
    template <class T>
    void dom_osm_object_history_extractor<T>::analyze(const osm_diff_analyzer_if::t_dom_tree & p_tree)
    {
      assert(!strcmp("osm",p_tree.getName()));
      int l_nb_version = p_tree.nChildNode();
      m_result = new std::vector<T*>();
      for(int l_index = 0; l_index < l_nb_version ; ++l_index)
	{
	  osm_diff_analyzer_if::t_dom_tree l_node = p_tree.getChildNode(l_index);
	  assert(!strcmp(l_node.getName(),T::get_type_str().c_str()));
	  m_result->push_back(generic_dom_utilities<osm_diff_analyzer_if::t_dom_tree>::extract_info<T>(l_node,true));
	}
    }
  
}
#endif // _DOM_OSM_OBJECT_HISTORY_EXTRACTOR_H_
//EOF
