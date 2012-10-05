#include "dom_parser.h"
#include "dom_analyser_operations.h"
#include <sstream>

using namespace std;



//------------------------------------------------------------------------------
//TO DELETEvoid dom_parser::add_analyser(dom_analyser_if & p_analyser)
//TO DELETE{
//TO DELETE  m_analysers.insert(&p_analyser);
//TO DELETE}
 
//------------------------------------------------------------------------------
void dom_parser::parse_stream(std::istream & p_stream)
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
  std::cout << "DOM PARSE SUCCESSFULL" << std::endl ;

  //TO DELETE  std::set<dom_analyser_if*>::iterator l_iter = m_analysers.begin();
  //TO DELETE  std::set<dom_analyser_if*>::iterator l_iter_end = m_analysers.end();
  //TO DELETE  while(l_iter != l_iter_end)
  //TO DELETE    {
  //TO DELETE      (*l_iter)->analyse(l_node);
  //TO DELETE      ++l_iter;
  //TO DELETE    }
  this->perform_analyse(dom_analyse_operation(l_node));
  std::cout << "End of DOM based analysers" << std::endl ;
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
