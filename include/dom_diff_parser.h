#ifndef _DOM_DIFF_PARSER_H_
#define _DOM_DIFF_PARSER_H_

#include "osm_diff_parser_base.h"
#include "dom_analyser_if.h"
#include <string>
#include <set>
#include <iostream>

namespace osm_diff_watcher
{
  class dom_diff_parser: public osm_diff_parser_base<dom_analyser_if>
  {
  public:

    //TO DELETE  void add_analyser(dom_analyser_if & p_analyser);
    void parse_stream(std::istream & p_stream);
  private:
    static void error_message(const XMLResults & p_err,std::string & p_msg);

    //TO DELETE  std::set<dom_analyser_if*> m_analysers;
  };
}
#endif // _DOM_DIFF_PARSER_H_
//EOF
