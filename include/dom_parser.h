#ifndef _DOM_PARSER_H_
#define _DOM_PARSER_H_

#include "xmlParser.h"
#include "dom_analyser_generic_if.h"
#include <string>
#include <set>
#include <iostream>


class dom_parser
{
 public:
  // Type defining interface of dom_analyser independantly of type used to represent the XML tree
  typedef dom_analyser_generic_if<XMLNode> dom_analyser_if;

  void add_analyser(dom_analyser_if & p_analyser);
  void parse(std::istream & p_stream);
 private:
  static void error_message(const XMLResults & p_err,std::string & p_msg);

  std::set<dom_analyser_if*> m_analysers;
};

#endif // _DOM_PARSER_H_
