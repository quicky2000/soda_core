#ifndef _OSM_DIFF_WATCHER_H_
#define _OSM_DIFF_WATCHER_H_

#include "module_manager.h"
#include "dom_analyzer_if.h"
#include "sax_analyzer_base.h"
#include "dom2cpp_analyzer.h"
#include "dom_parser.h"
#include "sax_parser.h"

#include <string>
#include <map>

namespace osm_diff_watcher
{
  class osm_diff_watcher
  {
  public:
    osm_diff_watcher(const std::string & p_file_name);
    ~osm_diff_watcher(void);
    void run(const uint64_t & p_start_seq_number);

  private:
    static void sig_handler(int p_sig);
    void dump_url(const std::string & p_url);
    void parse_diff(void);
    std::string get_url_diff(const uint64_t & p_seq_number);
    uint64_t get_sequence_number(void)const;

    static bool m_stop;
    module_manager m_module_manager;
    sax_parser m_sax_parser;
    dom_parser m_dom_parser;
    dom2cpp_analyzer m_dom2cpp_analyzer;
    std::map<std::string,osm_diff_analyzer_dom_if::dom_analyzer_if *> m_dom_analyzers;
    std::map<std::string,osm_diff_analyzer_sax_if::sax_analyzer_if *> m_sax_analyzers;
  };
}
#endif /* _OSM_DIFF_WATCHER_H_  */
//EOF
