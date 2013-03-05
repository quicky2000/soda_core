/*
  This file is part of osm_diff_watcher, Openstreetmap diff analyze framework
  The aim of this software is to provided generic facilities for diff analyzis
  to allow developers to concentrate on analyze rather than diff management 
  infrastructure
  Copyright (C) 2012  Julien Thevenon ( julien_thevenon at yahoo.fr )

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#ifndef _DOM_PARSER_BASE_H_
#define _DOM_PARSER_BASE_H_

#include "parser_base.h"
#include "xmlParser.h"
#include "quicky_exception.h"
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
        if(p_string != " " && p_string != "")
          {
            XMLResults l_err= {eXMLErrorNone,0,0};
            XMLNode l_node = XMLNode::parseString(p_string.c_str(),m_root.c_str(),&l_err);
            std::string l_error_msg;
            dom_parser_base<T>::error_message(l_err,l_error_msg);
            if(l_error_msg != "")
              {
                throw quicky_exception::quicky_logic_exception(l_error_msg,__LINE__,__FILE__);
              }
            this->perform_init();
            this->perform_dedicated_analyze(l_node);
          }
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
