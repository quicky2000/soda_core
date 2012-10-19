#ifndef _OSM_DIFF_WATCHER_H_
#define _OSM_DIFF_WATCHER_H_

#include "module_manager.h"
#include "dom_analyzer_if.h"
#include "sax_analyzer_base.h"

#include <string>
#include <map>

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

    module_manager m_module_manager;
    std::map<std::string,osm_diff_analyzer_dom_if::dom_analyzer_if *> m_dom_analyzers;
    std::map<std::string,osm_diff_analyzer_sax_if::sax_analyzer_base *> m_sax_analyzers;
  };
}
#endif /* _OSM_DIFF_WATCHER_H_  */
//EOF
