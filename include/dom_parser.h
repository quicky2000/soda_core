#ifndef _DOM_PARSER_H_
#define _DOM_PARSER_H_

#include "parser_base.h"
#include "dom_analyzer_if.h"
#include <string>
#include <set>
#include <iostream>

namespace osm_diff_watcher
{
  class dom_parser: public parser_base<osm_diff_analyzer_if::dom_analyzer_if>
  {
  public:
    dom_parser(const std::string & p_root="osmChange");
    void parse_stream(std::istream & p_stream);
    void parse_string(const std::string & p_string);
  private:
    std::string m_root;
    static void error_message(const XMLResults & p_err,std::string & p_msg);
  };
}
#endif // _DOM_PARSER_H_
//EOF
