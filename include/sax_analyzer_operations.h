#ifndef _SAX_ANALYZER_OPERATIONS_H_
#define _SAX_ANALYZER_OPERATIONS_H_

#include "sax_analyzer_if.h"
#include "analyze_operation.h"

namespace osm_diff_watcher
{
  class apply_init: public analyze_operation<sax_analyzer_if>
  {
  public:
    void operator()(sax_analyzer_if * p_analyzer)const
    {
      p_analyzer->init();
    }
  }
  ;

  class apply_element: public analyze_operation<sax_analyzer_if>
  {
  private:
    typedef void (sax_analyzer_if::* t_method_ptr)(const std::string &);
  public:
    apply_element(const char * p_element, t_method_ptr p_method_ptr):
      m_element(p_element),
      m_method_ptr(p_method_ptr)
	{
	}
      void operator()(sax_analyzer_if * p_analyzer)const
      {
	(p_analyzer->*m_method_ptr)(m_element);
      }
  private:
      const char * m_element;
      const t_method_ptr m_method_ptr;
  }
  ;

  class apply_attribute: public analyze_operation<sax_analyzer_if>
  {
  public:
    apply_attribute(const char * p_name,const char * p_value):
      m_name(p_name),
      m_value(p_value)
	{
	}
      void operator()(sax_analyzer_if * p_analyzer)const
      {
	p_analyzer->get_attribute(m_name,m_value);
      }
  private:
      const char * m_name;
      const char * m_value;
  }
  ;
}
#endif
//EOF
