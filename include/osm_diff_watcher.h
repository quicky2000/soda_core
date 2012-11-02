/*
    This file is part of osm_diff_watcher, Openstreetmap diff analyze framework
    The aim of this software is to provided generic facilities for diff analyzis
    to allow developers to concentrate on analyze rather than diff management 
    infrastructure
    Copyright (C) 2012  Julien Thevenon ( julien_thevenon at yahoo.fr )

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#ifndef _OSM_DIFF_WATCHER_H_
#define _OSM_DIFF_WATCHER_H_

#include "module_manager.h"
#include "dom_analyzer_if.h"
#include "sax_analyzer_if.h"
#include "cpp_analyzer_if.h"
#include "dom2cpp_analyzer.h"
#include "osm_diff_dom_parser.h"
#include "sax_parser.h"
#include "internal_database.h"
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
    void run(void);

  private:
    static void sig_handler(int p_sig);
    void parse_diff(const osm_diff_analyzer_if::osm_diff_state * p_diff_state);
    const uint64_t get_start_sequence_number(const osm_diff_analyzer_if::osm_diff_state &p_diff_state);
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
    internal_database m_informations;
  };
}
#endif /* _OSM_DIFF_WATCHER_H_  */
//EOF
