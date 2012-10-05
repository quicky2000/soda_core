#include "basic_dom_analyser.h"
#include <iostream>

//------------------------------------------------------------------------------
basic_dom_analyser::basic_dom_analyser(void):
  m_level(0)
{
}

//------------------------------------------------------------------------------
void basic_dom_analyser::analyse(const t_dom_tree & p_node)
{
  std::cout << "Level : " << m_level << std::endl ;
  std::string l_tab(m_level,'|');
  l_tab +="|=> ";
  std::cout << l_tab << "Element name : \"" << p_node.getName() << "\"" << std::endl ;
  int l_nb_text_field = p_node.nText();
  if(l_nb_text_field)
    {
      std::cout << l_tab << "Nb text fields : " << l_nb_text_field << std::endl ;
      for(int l_index=0;l_index<l_nb_text_field;++l_index)
        {
          std::cout << l_tab << "text_field[" << l_index << "] = \"" << p_node.getText(l_index) << "\"" << std::endl ;
        }
    }
  int l_nb_attribute = p_node.nAttribute();
  if(l_nb_attribute)
    {
      for(int l_index=0;l_index<l_nb_attribute;++l_index)
        {
          XMLAttribute l_attribute = p_node.getAttribute(l_index);
          std::cout << l_tab << "attribute[\"" << l_attribute.lpszName << "\"]=\"" << l_attribute.lpszValue << "\"" << std::endl ;
        }
    }

  int l_nb_child_node = p_node.nChildNode();
  std::cout << l_tab << "Nb child node : " << l_nb_child_node << std::endl ;
  for(int l_index=0;l_index<l_nb_child_node;++l_index)
    {
      XMLNode l_node = p_node.getChildNode(l_index);
      ++m_level;
      analyse(l_node);
      --m_level;
    }

}
//EOF
