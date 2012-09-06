#ifndef _SAX_PARSER_H_
#define _SAX_PARSER_H_

#include <expat.h>
#include <iostream>
#include <set>

class sax_analyser_if;

class sax_parser
{
public:
  // Callbacks for expat
  static void start(void *data, const char *el, const char **attr);
  static void end(void *data, const char *el);
  // End of callbacks

  sax_parser(void);
  ~sax_parser(void);
  void parse(std::istream & p_stream);
  void add_analyser(sax_analyser_if & p_analyser);
  
private:
  void analyse_start_element(const char *el, const char **attr);
  void analyse_end_element(const char *el);

  XML_Parser m_parser;
  std::set<sax_analyser_if*> m_analysers;
};

#endif /* _SAX_PARSER_H_ */
