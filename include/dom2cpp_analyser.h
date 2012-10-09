#ifndef _DOM2CPP_ANALYSER_H_
#define _DOM2CPP_ANALYSER_H_

#include "dom_analyser_if.h"

namespace osm_diff_watcher
{
  class dom2cpp_analyser: public dom_analyser_if
  {
  public:
    dom2cpp_analyser(void);
    void analyse(const t_dom_tree & p_tree);
  private:
  };
}
#endif
//EOF
