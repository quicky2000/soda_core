#ifndef _OSM_DIFF_WATCHER_H_
#define _OSM_DIFF_WATCHER_H_

#include "user_analyser.h"
#include <string>

namespace osm_diff_watcher
{
  class osm_diff_watcher
  {
  public:
    osm_diff_watcher(void);
    ~osm_diff_watcher(void);
    void run(const uint64_t & p_start_seq_number);

  private:
    void dump_url(const std::string & p_url);
    void parse_diff(void);
    std::string get_url_diff(const uint64_t & p_seq_number);
    uint64_t get_sequence_number(void)const;

    user_analyser m_analyser;
  };
}
#endif /* _OSM_DIFF_WATCHER_H_  */
//EOF
