#ifndef _DOM_DIFF_PARSER_H_
#define _DOM_DIFF_PARSER_H_

#include "parser_base.h"
#include "dom_analyser_if.h"
#include <string>
#include <set>
#include <iostream>

namespace osm_diff_watcher
{
  class dom_diff_parser: public parser_base<dom_analyser_if>
  {
  public:
    dom_diff_parser(const std::string & p_root="osmChange");
    void parse_stream(std::istream & p_stream);
    void parse_string(const std::string & p_string);
  private:
    std::string m_root;
    static void error_message(const XMLResults & p_err,std::string & p_msg);
  };
}
#endif // _DOM_DIFF_PARSER_H_
//EOF
