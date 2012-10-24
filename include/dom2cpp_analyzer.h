#ifndef _DOM2CPP_ANALYZER_H_
#define _DOM2CPP_ANALYZER_H_

#include "dom_analyzer_if.h"
#include "output_producer_if.h"
#include "cpp_analyzer_if.h"

namespace osm_diff_watcher
{
  class dom2cpp_analyzer: public osm_diff_analyzer_dom_if::dom_analyzer_if, public output_producer_if<osm_diff_analyzer_cpp_if::cpp_analyzer_if>
  {
  public:
    dom2cpp_analyzer(const std::string & p_name);
    // Method inherited from dom_analyzer_if
    inline const std::string & get_input_type(void)const;
    inline const std::string & get_output_type(void)const;
    inline const std::string & get_type(void)const;
    inline const std::string & get_name(void)const;
    void analyze(const osm_diff_analyzer_dom_if::t_dom_tree & p_tree);
    inline void init(void){}
    // end of Method inherited from dom_analyzer_if
  private:
    static const std::string m_input_type;
    static const std::string m_output_type;
    static const std::string m_type;
    const std::string m_name;
  };

  // Method inherited from dom_analyzer_if
  //----------------------------------------------------------------------------
  const std::string & dom2cpp_analyzer::get_input_type(void)const
    {
      return m_input_type;
    }
  //----------------------------------------------------------------------------
  const std::string & dom2cpp_analyzer::get_output_type(void)const
    {
      return m_output_type;
    }
  //----------------------------------------------------------------------------
  const std::string & dom2cpp_analyzer::get_type(void)const
    {
      return m_type;
    }
  //----------------------------------------------------------------------------
  const std::string & dom2cpp_analyzer::get_name(void)const
    {
      return m_name;
    }


}
#endif
//EOF
