#ifndef _SAX_PARSER_H_
#define _SAX_PARSER_H_

#include <expat.h>
#include "osm_diff_parser_base.h"
#include <iostream>
#include <set>


namespace osm_diff_watcher
{
  class sax_analyser_if;

  class sax_parser: public osm_diff_parser_base<sax_analyser_if>
  {
  public:
    // Callbacks for expat
    static void start(void *data, const char *el, const char **attr);
    static void end(void *data, const char *el);
    // End of callbacks

    sax_parser(void);
    ~sax_parser(void);
    void parse_stream(std::istream & p_stream);
    //TO DELETE   void add_analyser(sax_analyser_if & p_analyser);
  
  private:
    void analyse_start_element(const char *el, const char **attr);
    void analyse_end_element(const char *el);

    XML_Parser m_parser;
    //TO DELETE   std::set<sax_analyser_if*> m_analysers;
  };
}
#endif /* _SAX_PARSER_H_ */
