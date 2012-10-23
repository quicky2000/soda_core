#ifndef _SAX_PARSER_H_
#define _SAX_PARSER_H_

#include <expat.h>
#include "sax_analyzer_if.h"
#include "parser_base.h"
#include <iostream>
#include <set>


namespace osm_diff_watcher
{
  class sax_parser: public parser_base<osm_diff_analyzer_sax_if::sax_analyzer_if>
  {
  public:
    // Callbacks for expat
    static void start(void *data, const char *el, const char **attr);
    static void end(void *data, const char *el);
    // End of callbacks

    sax_parser(void);
    ~sax_parser(void);
  
  private:
    // Virtual method inherited from parser_base
    void parse_stream(std::istream & p_stream);
    // End of pure virtual methodes

    void analyze_start_element(const char *el, const char **attr);
    void analyze_end_element(const char *el);

    XML_Parser m_parser;
  };
}
#endif /* _SAX_PARSER_H_ */
