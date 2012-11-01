#ifndef _DOM_ANALYZER_OPERATIONS_H_
#define _DOM_ANALYZER_OPERATIONS_H_

#include "dom_analyzer_if.h"

namespace osm_diff_watcher
{
  class apply_init: public analyze_operation<osm_diff_analyzer_dom_if::dom_analyzer_if>
  {
  public:
    apply_init(const osm_diff_analyzer_if::osm_diff_state * p_state):
      m_state(p_state)
      {
        std::cout << "Apply init constructor " << m_state << std::endl ;
      }
    void operator()(osm_diff_analyzer_dom_if::dom_analyzer_if * p_analyzer)const
    {
      std::cout << "apply_init:operator() " << m_state << std::endl ;
      p_analyzer->init(m_state);
    }
  private:
    const osm_diff_analyzer_if::osm_diff_state * m_state;
  }
  ;

 class dom_analyze_operation: public analyze_operation<osm_diff_analyzer_dom_if::dom_analyzer_if>
  {
  public:
    dom_analyze_operation(const osm_diff_analyzer_dom_if::t_dom_tree & p_tree):
      m_tree(p_tree)
      {
      }

      void operator()(osm_diff_analyzer_dom_if::dom_analyzer_if * p_analyzer)const
      {
	p_analyzer->analyze(m_tree);
      }
  private:
      const osm_diff_analyzer_dom_if::t_dom_tree & m_tree;
  }
  ;
}
#endif
//EOF
