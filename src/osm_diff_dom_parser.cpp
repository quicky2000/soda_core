#include "osm_diff_dom_parser.h"
#include "dom_analyzer_operations.h"
#include <sstream>

using namespace std;

namespace osm_diff_watcher
{
   //------------------------------------------------------------------------------
  void osm_diff_dom_parser::perform_init(void)
  {
    this->perform_analyze(apply_init(get_diff_state()));
  }
  //------------------------------------------------------------------------------
  void osm_diff_dom_parser::perform_dedicated_analyze(const XMLNode & p_node)
  {
    this->perform_analyze(dom_analyze_operation(p_node));
  }
  
  //------------------------------------------------------------------------------
  osm_diff_dom_parser::osm_diff_dom_parser(const std::string & p_root):
    dom_parser_base<osm_diff_analyzer_dom_if::dom_analyzer_if>(p_root)
  {
  }

}

//EOF
