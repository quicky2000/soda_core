#include "sax_parser.h"
#include "sax_analyser_if.h"
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <assert.h>

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
void sax_parser::parse(std::istream & p_stream)
{
  std::set<sax_analyser_if*>::iterator l_iter = m_analysers.begin();
  std::set<sax_analyser_if*>::iterator l_iter_end = m_analysers.end();
  while(l_iter != l_iter_end)
    {
      (*l_iter)->init();
      ++l_iter;
    }
//   std::ifstream l_file(m_name.c_str());
//   if(!l_file)
//     {
//       std::cout << "ERROR : Could not open file \"" << m_name << "\"" << std::endl ;
//       exit(-1);
//     }
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
  std::cout << "PARSE SUCCESSFULL" << std::endl ;
}

//------------------------------------------------------------------------------
void sax_parser::start(void *p_data, const char *p_element, const char **p_attribute)
{
  sax_parser *l_parser = static_cast<sax_parser*>(p_data);
  assert(l_parser);
  l_parser->analyse_start_element(p_element,p_attribute);

}

//------------------------------------------------------------------------------
void sax_parser::end(void *p_data, const char *p_element)
{
  sax_parser *l_parser = static_cast<sax_parser*>(p_data);
  assert(l_parser);
  l_parser->analyse_end_element(p_element);
}

//------------------------------------------------------------------------------
void sax_parser::add_analyser(sax_analyser_if & p_analyser)
{
  m_analysers.insert(&p_analyser);
}

//------------------------------------------------------------------------------
void sax_parser::analyse_start_element(const char *p_element, const char **p_attribute)
{
  std::set<sax_analyser_if*>::iterator l_iter = m_analysers.begin();
  std::set<sax_analyser_if*>::iterator l_iter_end = m_analysers.end();
  while(l_iter != l_iter_end)
    {
      (*l_iter)->start_element(p_element);
      for (uint32_t i = 0; p_attribute[i]; i += 2)
	{
	  (*l_iter)->get_attribute(p_attribute[i],p_attribute[i+1]);
	}
     ++l_iter;
    }
 
}

//------------------------------------------------------------------------------
void sax_parser::analyse_end_element(const char *p_element)
{
  std::set<sax_analyser_if*>::iterator l_iter = m_analysers.begin();
  std::set<sax_analyser_if*>::iterator l_iter_end = m_analysers.end();
  while(l_iter != l_iter_end)
    {
      (*l_iter)->end_element(p_element);
      ++l_iter;
    }
}

//EOF
