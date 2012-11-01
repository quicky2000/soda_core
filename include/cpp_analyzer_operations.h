#ifndef _CPP_ANALYZER_OPERATIONS_H_
#define _CPP_ANALYZER_OPERATIONS_H_

#include "cpp_analyzer_if.h"

namespace osm_diff_watcher
{
  class apply_init: public analyze_operation<osm_diff_analyzer_cpp_if::cpp_analyzer_if>
  {
  public:
    apply_init(const osm_diff_analyzer_if::osm_diff_state * p_state):
      m_state(p_state)
      {
      }
    void operator()(osm_diff_analyzer_cpp_if::cpp_analyzer_if * p_analyzer)const
    {
      p_analyzer->init(m_state);
    }
  private:
    const osm_diff_analyzer_if::osm_diff_state * m_state;
  }
  ;

 class apply_changes: public analyze_operation<osm_diff_analyzer_cpp_if::cpp_analyzer_if>
  {
  public:
    apply_changes(const std::vector<osm_api_data_types::osm_change*> & p_changes):
      m_changes(p_changes)
      {
      }

      void operator()(osm_diff_analyzer_cpp_if::cpp_analyzer_if * p_analyzer)const
      {
	p_analyzer->analyze(m_changes);
      }
  private:
      const std::vector<osm_api_data_types::osm_change*> & m_changes;
  }
  ;
}
#endif
//EOF
