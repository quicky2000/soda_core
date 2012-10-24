#ifndef _OUTPUT_PRODUCER_IF_
#define _OUTPUT_PRODUCER_IF_

#include "analyze_operation.h"
#include <set>

namespace osm_diff_watcher
{
  template <class T>
    class output_producer_if
    {
    public:
      void add_analyzer(T & p_analyzer);
    protected:
      void perform_analyze(const analyze_operation<T> & f);
    private:
      std::set<T*> m_analyzers;
  };

  //------------------------------------------------------------------------------
  template <class T>
    void output_producer_if<T>::add_analyzer(T & p_analyzer)
    {
      m_analyzers.insert(&p_analyzer);
    }

  //------------------------------------------------------------------------------
  template <class T>
    void output_producer_if<T>::perform_analyze(const analyze_operation<T> & func)
    {
      for(typename std::set<T*>::iterator l_iter = m_analyzers.begin();l_iter != m_analyzers.end() ; ++l_iter)
	{
	  func(*l_iter);
	}
    }

}


#endif // _OUTPUT_PRODUCER_IF_
//EOF
