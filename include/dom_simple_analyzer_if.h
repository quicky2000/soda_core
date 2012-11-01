#ifndef _DOM_SIMPLE_ANALYZER_IF_H_
#define _DOM_SIMPLE_ANALYZER_IF_H_

#include "dom_analyzer_generic_if.h"
#include "dom_tree.h"
#include "xmlParser.h"

namespace osm_diff_watcher
{
  class dom_simple_analyzer_if: public osm_diff_analyzer_dom_if::dom_analyzer_generic_if<XMLNode>
  {
  public:
    virtual void init(void)=0;
    inline ~dom_simple_analyzer_if(void){}
 };
}

#endif // _DOM_SIMPLE_ANALYZER_IF_H_
//EOF
