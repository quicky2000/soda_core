#include "dom_parser.h"
#include "dom_analyzer_operations.h"
#include <sstream>

using namespace std;

namespace osm_diff_watcher
{
  //------------------------------------------------------------------------------
  dom_parser::dom_parser(const std::string & p_root):
    m_root(p_root)
  {
  }

  //------------------------------------------------------------------------------
  void dom_parser::parse_stream(std::istream & p_stream)
  {
    std::string l_xml_string;
    std::string l_line;
    while(!getline(p_stream,l_line).eof())
      {
	l_xml_string += l_line;
      }
    parse_string(l_xml_string);
  }

  //------------------------------------------------------------------------------
  void dom_parser::parse_string(const std::string & p_string)
  {
    XMLResults l_err= {eXMLErrorNone,0,0};
    XMLNode l_node = XMLNode::parseString(p_string.c_str(),m_root.c_str(),&l_err);
    std::string l_error_msg;
    dom_parser::error_message(l_err,l_error_msg);
    if(l_error_msg != "")
      {
	std::cout << l_error_msg << std::endl ;
	exit(-1);
      }
    std::cout << "DOM PARSE SUCCESSFULL" << std::endl ;

    this->perform_analyze(apply_init());
    this->perform_analyze(dom_analyze_operation(l_node));

    std::cout << "End of DOM based analyzers" << std::endl ;
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
	p_msg = "dom_parser : \"" + p_msg+ "\" on line " + l_line_number.str() + " when parsing XML column " + l_column_number.str();
      }
    else
      {
	p_msg = "";
      }
  }
}

//EOF
