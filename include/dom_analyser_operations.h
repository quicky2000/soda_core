#ifndef _DOM_ANALYSER_OPERATIONS_H_
#define _DOM_ANALYSER_OPERATIONS_H_

#include "dom_analyser_if.h"

namespace osm_diff_watcher
{
  class dom_analyse_operation: public analyse_operation<dom_analyser_if>
  {
  public:
    dom_analyse_operation(const t_dom_tree & p_tree):
      m_tree(p_tree)
      {
      }

      void operator()(dom_analyser_if * p_analyser)const
      {
	p_analyser->analyse(m_tree);
      }
  private:
      const t_dom_tree & m_tree;
  }
  ;
}
#endif
//EOF
