#ifndef _DOM_PARSER_BASE_H_
#define _DOM_PARSER_BASE_H_

#include "parser_base.h"
#include "xmlParser.h"
#include <sstream>

namespace osm_diff_watcher
{
  template <class T>
    class dom_parser_base: public parser_base<T>
  {
  public:
    dom_parser_base(const std::string & p_root);
    void parse_stream(std::istream & p_stream);
    void parse_string(const std::string & p_string);

    inline virtual ~dom_parser_base(void){}
  private:
    virtual void perform_init(void)=0;
    virtual void perform_dedicated_analyze(const XMLNode & p_node)=0;
 
    std::string m_root;
    static void error_message(const XMLResults & p_err,std::string & p_msg);
  };

  //------------------------------------------------------------------------------
  template <class T>
  dom_parser_base<T>::dom_parser_base(const std::string & p_root):
    m_root(p_root)
      {
      }
  //------------------------------------------------------------------------------
  template <class T>
  void dom_parser_base<T>::parse_stream(std::istream & p_stream)
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
  template <class T>
  void dom_parser_base<T>::parse_string(const std::string & p_string)
  {
    XMLResults l_err= {eXMLErrorNone,0,0};
    XMLNode l_node = XMLNode::parseString(p_string.c_str(),m_root.c_str(),&l_err);
    std::string l_error_msg;
    dom_parser_base<T>::error_message(l_err,l_error_msg);
    if(l_error_msg != "")
      {
	std::cout << l_error_msg << std::endl ;
	exit(-1);
      }
    std::cout << "DOM PARSE SUCCESSFULL" << std::endl ;

    this->perform_init();
    this->perform_dedicated_analyze(l_node);

    std::cout << "End of DOM based analyzers" << std::endl ;
  }

  //------------------------------------------------------------------------------
  template <class T>
  void dom_parser_base<T>::error_message(const XMLResults & p_err,std::string & p_msg)
  {
    p_msg = XMLNode::getError(p_err.error);
    std::stringstream l_line_number ;
    l_line_number << p_err.nLine;
    std::stringstream l_column_number ;
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

#endif // _DOM_PARSER_BASE_H_
//EOF
