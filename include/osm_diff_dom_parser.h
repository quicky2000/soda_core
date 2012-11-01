#ifndef _OSM_DIFF_DOM_PARSER_H_
#define _OSM_DIFF_DOM_PARSER_H_

#include "dom_parser_base.h"
#include "dom_analyzer_if.h"
#include "osm_diff_parser_if.h"

namespace osm_diff_watcher
{
  class osm_diff_dom_parser: public dom_parser_base<osm_diff_analyzer_dom_if::dom_analyzer_if>, public osm_diff_parser_if
  {
  public:
    osm_diff_dom_parser(const std::string & p_root="osmChange");
  private:
    void perform_init(void);
    void perform_dedicated_analyze(const XMLNode & p_node);
  };

};
#endif // _OSM_DIFF_DOM_PARSER_H_
//EOF
