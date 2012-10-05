#ifndef _SAX_ANALYSER_OPERATIONS_H_
#define _SAX_ANALYSER_OPERATIONS_H_

#include "sax_analyser_if.h"
#include "analyse_operation.h"

namespace osm_diff_watcher
{
  class apply_init: public analyse_operation<sax_analyser_if>
  {
  public:
    void operator()(sax_analyser_if * p_analyser)const
    {
      p_analyser->init();
    }
  }
  ;

  class apply_element: public analyse_operation<sax_analyser_if>
  {
  private:
    typedef void (sax_analyser_if::* t_method_ptr)(const std::string &);
  public:
    apply_element(const char * p_element, t_method_ptr p_method_ptr):
      m_element(p_element),
      m_method_ptr(p_method_ptr)
	{
	}
      void operator()(sax_analyser_if * p_analyser)const
      {
	(p_analyser->*m_method_ptr)(m_element);
      }
  private:
      const char * m_element;
      const t_method_ptr m_method_ptr;
  }
  ;

  class apply_attribute: public analyse_operation<sax_analyser_if>
  {
  public:
    apply_attribute(const char * p_name,const char * p_value):
      m_name(p_name),
      m_value(p_value)
	{
	}
      void operator()(sax_analyser_if * p_analyser)const
      {
	p_analyser->get_attribute(m_name,m_value);
      }
  private:
      const char * m_name;
      const char * m_value;
  }
  ;
}
#endif
//EOF
