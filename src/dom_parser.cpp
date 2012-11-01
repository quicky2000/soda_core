#include "dom_parser.h"
#include "dom_simple_analyzer_operations.h"
#include <sstream>

using namespace std;

namespace osm_diff_watcher
{
  //------------------------------------------------------------------------------
  dom_parser::dom_parser(const std::string & p_root):
    dom_parser_base<dom_simple_analyzer_if>(p_root)
  {
  }

  //------------------------------------------------------------------------------
  void dom_parser::perform_dedicated_analyze(const XMLNode & p_node)
  {
    this->perform_analyze(dom_simple_analyze_operation(p_node));
  }

  //------------------------------------------------------------------------------
  void dom_parser::perform_init(void)
  {
    this->perform_analyze(apply_simple_init());
  }

}

//EOF
