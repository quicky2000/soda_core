#ifndef _DIFF_PARSER_IF_H_
#define _DIFF_PARSER_IF_H_

#include <cassert>

namespace osm_diff_analyzer_if
{
  class osm_diff_state;
}

namespace osm_diff_watcher
{
  class osm_diff_parser_if
  {
  public:
    inline osm_diff_parser_if(void);
    inline void set_diff_state(const osm_diff_analyzer_if::osm_diff_state * p_diff_state);
    inline const osm_diff_analyzer_if::osm_diff_state * get_diff_state(void)const;
  private:
    const osm_diff_analyzer_if::osm_diff_state * m_diff_state;
  };

  //----------------------------------------------------------------------------
  osm_diff_parser_if::osm_diff_parser_if(void):
    m_diff_state(NULL)
    {
    }
    //----------------------------------------------------------------------------
    void osm_diff_parser_if::set_diff_state(const osm_diff_analyzer_if::osm_diff_state * p_diff_state)
    {
      assert(p_diff_state);
      m_diff_state = p_diff_state;
    }
    //----------------------------------------------------------------------------
    const osm_diff_analyzer_if::osm_diff_state * osm_diff_parser_if::get_diff_state(void)const
      {
        return m_diff_state;
      }
}
#endif // _OSM_DIFF_PARSER_IF_H_
//EOF
