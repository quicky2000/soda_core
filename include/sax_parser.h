#ifndef _SAX_PARSER_H_
#define _SAX_PARSER_H_

#include <expat.h>
#include "parser_base.h"
#include <iostream>
#include <set>


namespace osm_diff_watcher
{
  class sax_analyzer_if;

  class sax_parser: public parser_base<sax_analyzer_if>
  {
  public:
    // Callbacks for expat
    static void start(void *data, const char *el, const char **attr);
    static void end(void *data, const char *el);
    // End of callbacks

    sax_parser(void);
    ~sax_parser(void);
    void parse_stream(std::istream & p_stream);
    //TO DELETE   void add_analyzer(sax_analyzer_if & p_analyzer);
  
  private:
    void analyze_start_element(const char *el, const char **attr);
    void analyze_end_element(const char *el);

    XML_Parser m_parser;
    //TO DELETE   std::set<sax_analyzer_if*> m_analyzers;
  };
}
#endif /* _SAX_PARSER_H_ */
