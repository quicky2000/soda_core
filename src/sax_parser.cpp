#include "sax_parser.h"
#include "sax_analyzer_operations.h"
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <assert.h>

namespace osm_diff_watcher
{
  //------------------------------------------------------------------------------
  sax_parser::sax_parser(void):
    m_parser(XML_ParserCreate(NULL))
  {
    if (!m_parser)
      {
	std::cout << "Couldn't allocate memory for parser" << std::endl ;
	exit(-1);
      }

    XML_SetUserData(m_parser,this);
    XML_SetElementHandler(m_parser,start, end);
  }

  //------------------------------------------------------------------------------
  sax_parser::~sax_parser(void)
  {
    XML_ParserFree(m_parser);
  }

  //------------------------------------------------------------------------------
  void sax_parser::parse_stream(std::istream & p_stream)
  {
    this->perform_analyze(apply_init());

    const uint32_t l_size = 10000;
    char l_buf[l_size];
    bool l_end = false;
    while(! (l_end = p_stream.eof()))
      {
	p_stream.read(l_buf,l_size);
	if (! XML_Parse(m_parser, l_buf,p_stream.gcount(),l_end))
	  {
	    std::cout << "ERROR : Parse error at line " << XML_GetCurrentLineNumber(m_parser) << " :" << XML_ErrorString(XML_GetErrorCode(m_parser)) << std::endl;
	    exit(-1);
	  }
      }
    std::cout << "SAX PARSE SUCCESSFULL" << std::endl ;
  }

  //------------------------------------------------------------------------------
  void sax_parser::start(void *p_data, const char *p_element, const char **p_attribute)
  {
    sax_parser *l_parser = static_cast<sax_parser*>(p_data);
    assert(l_parser);
    l_parser->analyze_start_element(p_element,p_attribute);

  }

  //------------------------------------------------------------------------------
  void sax_parser::end(void *p_data, const char *p_element)
  {
    sax_parser *l_parser = static_cast<sax_parser*>(p_data);
    assert(l_parser);
    l_parser->analyze_end_element(p_element);
  }

  //------------------------------------------------------------------------------
  void sax_parser::analyze_start_element(const char *p_element, const char **p_attribute)
  {
    this->perform_analyze(apply_element(p_element,&osm_diff_analyzer_sax_if::sax_analyzer_if::start_element));
    for (uint32_t i = 0; p_attribute[i]; i += 2)
      {
	this->perform_analyze(apply_attribute(p_attribute[i],p_attribute[i+1]));
      }
  }

  //------------------------------------------------------------------------------
  void sax_parser::analyze_end_element(const char *p_element)
  {
    this->perform_analyze(apply_element(p_element,&osm_diff_analyzer_sax_if::sax_analyzer_if::end_element));
  }
}
//EOF
