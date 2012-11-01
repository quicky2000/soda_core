#ifndef _OSM_DIFF_WATCHER_H_
#define _OSM_DIFF_WATCHER_H_

#include "module_manager.h"
#include "dom_analyzer_if.h"
#include "sax_analyzer_if.h"
#include "cpp_analyzer_if.h"
#include "dom2cpp_analyzer.h"
#include "osm_diff_dom_parser.h"
#include "sax_parser.h"
#include <string>
#include <map>

namespace quicky_url_reader
{
  class url_reader;
}

namespace osm_diff_watcher
{
  class configuration;
  class osm_ressources;
  class common_api_wrapper;

  class osm_diff_watcher
  {
  public:
    osm_diff_watcher(const std::string & p_file_name);
    ~osm_diff_watcher(void);
    void run(const uint64_t & p_start_seq_number);

  private:
    static void sig_handler(int p_sig);
    //TO DELETE    void dump_url(const std::string & p_url);
    void parse_diff(const osm_diff_analyzer_if::osm_diff_state * p_diff_state);
    //TO DELETE    std::string get_url_diff(const uint64_t & p_seq_number);
    //TO DELETE    uint64_t get_sequence_number(void)const;

    static bool m_stop;

    osm_ressources & m_ressources;
    common_api_wrapper & m_api_wrapper;
    std::string m_diff_file_name;
    const configuration * m_configuration;
    module_manager m_module_manager;
    sax_parser m_sax_parser;
    osm_diff_dom_parser m_dom_parser;
    dom2cpp_analyzer m_dom2cpp_analyzer;
    std::map<std::string,osm_diff_analyzer_dom_if::dom_analyzer_if *> m_dom_analyzers;
    std::map<std::string,osm_diff_analyzer_sax_if::sax_analyzer_if *> m_sax_analyzers;
    std::map<std::string,osm_diff_analyzer_cpp_if::cpp_analyzer_if *> m_cpp_analyzers;
    std::map<std::string,osm_diff_analyzer_if::diff_analyzer_if *> m_disabled_analyzers;
    quicky_url_reader::url_reader & m_url_reader;
  };
}
#endif /* _OSM_DIFF_WATCHER_H_  */
//EOF
