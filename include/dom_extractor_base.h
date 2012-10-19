#ifndef _DOM_EXTRACTOR_BASE_H_
#define _DOM_EXTRACTOR_BASE_H_

#include "dom_analyzer_if.h"

namespace osm_diff_watcher
{
  class dom_extractor_base: public osm_diff_analyzer_dom_if::dom_analyzer_if
  {
  public:
    inline dom_extractor_base(const std::string & p_type);

    // Method inherited from dom_analyzer_if
    inline const std::string & get_input_type(void)const;
    inline const std::string & get_output_type(void)const;
    inline const std::string & get_type(void)const;
    inline const std::string & get_name(void)const;
    // end of Method inherited from dom_analyzer_if
  private:
    const std::string m_input_type;
    const std::string m_output_type;
    const std::string m_type;
    const std::string m_name;
  };

  dom_extractor_base::dom_extractor_base(const std::string & p_type):
    m_input_type("dom"),
    m_output_type(""),
    m_type(p_type),
    m_name(p_type+"_instance")
    {
    }

    // Method inherited from dom_analyzer_if
    const std::string & dom_extractor_base::get_input_type(void)const
      {
        return m_input_type;
      }
    const std::string & dom_extractor_base::get_output_type(void)const
      {
        return m_output_type;
      }
    const std::string & dom_extractor_base::get_type(void)const
      {
        return m_type;
      }
    const std::string & dom_extractor_base::get_name(void)const
      {
        return m_name;
      }
    // end of Method inherited from dom_analyzer_if
}
#endif // _DOM_EXTRACTOR_BASE_H_
//EOF
