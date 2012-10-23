#ifndef _DOM_CONFIGURATION_EXTRACTOR_H_
#define _DOM_CONFIGURATION_EXTRACTOR_H_

#include "dom_extractor_base.h"
#include "configuration.h"
#include "xmlParser.h"
#include <cstring>

namespace osm_diff_watcher
{
  class dom_configuration_extractor:public dom_extractor_base
  {
  public:
    inline dom_configuration_extractor(void);
    // Method inherited from dom_analyzer_if
    inline void analyze(const osm_diff_analyzer_dom_if::t_dom_tree & p_tree);
    // end of Method inherited from dom_analyzer_if
    inline const configuration * get_result(void)const;
  private:
    configuration * m_result;
  };
  //----------------------------------------------------------------------------
  dom_configuration_extractor::dom_configuration_extractor(void):
    dom_extractor_base("configuration_extractor"),
    m_result(NULL)
    {
    }
    //--------------------------------------------------------------------------
    const configuration * dom_configuration_extractor::get_result(void)const
      {
	return m_result;
      }
    //--------------------------------------------------------------------------
    void dom_configuration_extractor::analyze(const osm_diff_analyzer_dom_if::t_dom_tree & p_tree)
    {
      assert(!strcmp("osm_diff_watcher_configuration",p_tree.getName()));
      m_result = new configuration();
      int l_nb_nodes = p_tree.nChildNode();
      for(int l_index = 0; l_index < l_nb_nodes ; ++l_index)
	{
	  osm_diff_analyzer_dom_if::t_dom_tree l_node = p_tree.getChildNode(l_index);
	  if(!strcmp(l_node.getName(),"library"))
	    {
	      XMLCSTR l_library_name = l_node.getAttribute("name");
              assert(l_library_name);
	      m_result->add_library(l_library_name);
	    }
	  else if(!strcmp(l_node.getName(),"analyzer"))
            {
	      XMLCSTR l_module_type = l_node.getAttribute("type");
              assert(l_module_type);
	      XMLCSTR l_module_name = l_node.getAttribute("name");
              assert(l_module_name);
	      m_result->add_module(l_module_name,l_module_type);
            }
	  else if(!strcmp(l_node.getName(),"variable"))
            {
	      XMLCSTR l_variable_name = l_node.getAttribute("name");
              assert(l_variable_name);
	      XMLCSTR l_variable_value = l_node.getAttribute("value");
              assert(l_variable_value);
	      m_result->add_variable(l_variable_name,l_variable_value);
            }
          else
	    {
	      std::cout << "ERROR : unknown configuration item \"" << l_node.getName() << "\"" << std::endl ;
	      exit(-1);
	    }
	}
   }

}
#endif // _DOM_CONFIGURATION_EXTRACTOR_H_
//EOF
