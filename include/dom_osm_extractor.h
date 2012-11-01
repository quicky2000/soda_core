#ifndef _DOM_OSM_EXTRACTOR_H_
#define _DOM_OSM_EXTRACTOR_H_

#include "dom_simple_analyzer_if.h"
#include "dom_generic_utilities.h"
#include <cstring>

namespace osm_diff_watcher
{
  template <class T>
  class dom_osm_extractor: public dom_simple_analyzer_if
  {
  public:
    dom_osm_extractor(void);
    // Method inherited from dom_analyzer_if
    void analyze(const osm_diff_analyzer_dom_if::t_dom_tree & p_tree);
    inline void init(void){}
    // end of Method inherited from dom_analyzer_if
    T * get_result(void)const;
  private:
    T * m_result;
  }
  ;

  //----------------------------------------------------------------------------
  template <class T>
    dom_osm_extractor<T>::dom_osm_extractor(void):
    m_result(NULL)
    {
    }

  //----------------------------------------------------------------------------
  template <class T>
    T * dom_osm_extractor<T>::get_result(void)const
    {
      return m_result;
    }

  //----------------------------------------------------------------------------
    template <class T>
    void dom_osm_extractor<T>::analyze(const osm_diff_analyzer_dom_if::t_dom_tree & p_tree)
    {
      assert(!strcmp("osm",p_tree.getName()));
      assert(p_tree.nChildNode()==1);
      osm_diff_analyzer_dom_if::t_dom_tree l_node = p_tree.getChildNode(0);
      assert(!strcmp(l_node.getName(),T::get_type_str().c_str()));
      m_result = generic_dom_utilities<osm_diff_analyzer_dom_if::t_dom_tree>::extract_info<T>(l_node,true);
    }
  
}
#endif // _DOM_OSM_EXTRACTOR_H_
//EOF
