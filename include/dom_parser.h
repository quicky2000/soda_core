#ifndef _DOM_PARSER_H_
#define _DOM_PARSER_H_

#include "dom_parser_base.h"
#include "dom_simple_analyzer_if.h"

namespace osm_diff_watcher
{
  class dom_parser: public dom_parser_base<dom_simple_analyzer_if>
  {
  public:
    dom_parser(const std::string & p_root); 
  private:
    void perform_init(void);
    void perform_dedicated_analyze(const XMLNode & p_node);
  };

  
}
#endif // _DOM_PARSER_H_
//EOF
