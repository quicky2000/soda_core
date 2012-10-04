#include "dom_parser.h"
#include <sstream>

using namespace std;

//------------------------------------------------------------------------------
void dom_parser::add_analyser(dom_analyser_if & p_analyser)
{
  m_analysers.insert(&p_analyser);
}
 
//------------------------------------------------------------------------------
void dom_parser::parse(std::istream & p_stream)
{
  std::string l_xml_string;
  std::string l_line;
  while(!getline(p_stream,l_line).eof())
    {
      l_xml_string += l_line;
    }
  XMLResults l_err= {eXMLErrorNone,0,0};
  XMLNode l_node = XMLNode::parseString(l_xml_string.c_str(),"osmChange",&l_err);
  std::string l_error_msg;
  dom_parser::error_message(l_err,l_error_msg);
  if(l_error_msg != "")
    {
      std::cout << l_error_msg << std::endl ;
      exit(-1);
    }
  std::set<dom_analyser_if*>::iterator l_iter = m_analysers.begin();
  std::set<dom_analyser_if*>::iterator l_iter_end = m_analysers.end();
  while(l_iter != l_iter_end)
    {
      (*l_iter)->analyse(l_node);
      ++l_iter;
    }

}

//------------------------------------------------------------------------------
void dom_parser::error_message(const XMLResults & p_err,std::string & p_msg)
{
  p_msg = XMLNode::getError(p_err.error);
  stringstream l_line_number ;
  l_line_number << p_err.nLine;
  stringstream l_column_number ;
  l_column_number << p_err.nColumn;


  if(p_msg != "No error")
    {
      p_msg = "Dom_parser : \"" + p_msg+ "\" on line " + l_line_number.str() + " when parsing XML column " + l_column_number.str();
    }
  else
    {
      p_msg = "";
    }
}
//EOF
