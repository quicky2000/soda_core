#include "osm_diff_watcher.h"
#include "url_reader.h"
#include "download_buffer.h"
#include "sax_parser.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>
#include "gzstream.h"
#include "dom_parser.h"
#include "dom2cpp_analyzer.h"

using namespace quicky_url_reader;

namespace osm_diff_watcher
{
  //------------------------------------------------------------------------------
  osm_diff_watcher::osm_diff_watcher(void)
  {
    m_module_manager.load_library("../osm_diff_analyzer_test_dom/bin/libosm_diff_analyzer_test_dom.so");
    osm_diff_analyzer_if::dom_analyzer_if * l_dom_analyzer = m_module_manager.create_module<osm_diff_analyzer_if::dom_analyzer_if>("test_dom","test_dom_instance");
    m_dom_analyzers.insert(make_pair(l_dom_analyzer->get_name(),l_dom_analyzer));

    m_module_manager.load_library("../osm_diff_analyzer_new_user/bin/libosm_diff_analyzer_new_user.so");
    osm_diff_analyzer_if::sax_analyzer_base * l_sax_analyzer = m_module_manager.create_module<osm_diff_analyzer_if::sax_analyzer_base>("new_user","new_user_instance");
     m_sax_analyzers.insert(make_pair(l_sax_analyzer->get_name(),l_sax_analyzer));
  }

  //------------------------------------------------------------------------------
  osm_diff_watcher::~osm_diff_watcher(void)
  {
    for(std::map<std::string,osm_diff_analyzer_if::dom_analyzer_if *>::iterator l_iter = m_dom_analyzers.begin();
        l_iter != m_dom_analyzers.end();
        ++l_iter)
      {
        delete l_iter->second;
      }

    for(std::map<std::string,osm_diff_analyzer_if::sax_analyzer_base *>::iterator l_iter = m_sax_analyzers.begin();
        l_iter != m_sax_analyzers.end();
        ++l_iter)
      {
        delete l_iter->second;
      }

  }


  //------------------------------------------------------------------------------
  void osm_diff_watcher::run(const uint64_t & p_start_seq_number)
  {
    uint64_t l_end_seq_number = get_sequence_number();
    uint64_t l_current_seq_number = ( p_start_seq_number ? p_start_seq_number : l_end_seq_number);
    uint32_t l_nb_iteration = 3;//00 ;
    while(l_nb_iteration)
      {
	std::cout << "--------------------------------------------------------" << std::endl ;
	std::cout << l_nb_iteration << " remaining iterations" << std::endl ;
	std::cout << "Latest seq number available = " << l_end_seq_number << std::endl ;
	std::cout << "--> Sequence number = \"" << l_current_seq_number << "\"" << std::endl;
	std::string l_url_diff = get_url_diff(l_current_seq_number);
	std::cout << "Url of diff file \"" << l_url_diff << "\"" << std::endl ;
	dump_url(l_url_diff);
	parse_diff();

	if(l_current_seq_number == l_end_seq_number)
	  {
	    // Waiting for new end sequence number
	    uint32_t l_delay = 1;
	    do
	      {
		std::cout << "Wait for " << l_delay << " seconds" << std::endl ;
		sleep(l_delay);
		l_end_seq_number = get_sequence_number();     
		if(l_delay < 30 ) l_delay *= 2;
	      }while(l_current_seq_number == l_end_seq_number);
	  }
      
	++l_current_seq_number;
	--l_nb_iteration;
      }
  }

  //------------------------------------------------------------------------------
  void osm_diff_watcher::parse_diff(void)
  {
    // Sax analyze
    sax_parser l_sax_parser;

    // Add loaded SAX parsers
    for(std::map<std::string,osm_diff_analyzer_if::sax_analyzer_base *>::iterator l_iter = m_sax_analyzers.begin();
        l_iter != m_sax_analyzers.end();
        ++l_iter)
      {
        l_sax_parser.add_analyzer(*(l_iter->second));
      }

    // Add built-in SAX parsers

    l_sax_parser.parse("tmp_diff.gz");

    // DOM analyze
    dom_parser l_dom_parser;

    // Add loaded DOM parsers
    for(std::map<std::string,osm_diff_analyzer_if::dom_analyzer_if *>::iterator l_iter = m_dom_analyzers.begin();
        l_iter != m_dom_analyzers.end();
        ++l_iter)
      {
        l_dom_parser.add_analyzer(*(l_iter->second));
      }
    // Add built-in DOM parsers
    dom2cpp_analyzer l_dom2cpp_analyzer("dom2cpp_analyzer_instance");
    l_dom_parser.add_analyzer(l_dom2cpp_analyzer);

    l_dom_parser.parse("tmp_diff.gz");  
  }

  //------------------------------------------------------------------------------
  void osm_diff_watcher::dump_url(const std::string & p_url)
  {
    url_reader l_url_reader;
    download_buffer l_buffer;
    l_url_reader.read_url(p_url.c_str(),l_buffer);
    std::ofstream l_output_file("tmp_diff.gz",std::ios::out | std::ios::binary);
    if(l_output_file==NULL)
      {
	std::cout << "ERROR : Unable to open output file \"tmp_diff.gz\"" << std::endl ;
	exit(EXIT_FAILURE);
      }
    l_output_file.write(l_buffer.get_data(),l_buffer.get_size());
    l_output_file.close();
  }

  //------------------------------------------------------------------------------
  std::string osm_diff_watcher::get_url_diff(const uint64_t & p_seq_number)
  {
    std::stringstream l_stream;
    l_stream << p_seq_number;
    std::string l_seq_number = l_stream.str();
    std::string l_complete_seq_number = (l_seq_number.size() < 9 ? std::string(9 - l_seq_number.size(),'0') + l_seq_number : l_seq_number);
    // For more information refer to OSM wiki page
    // http://wiki.openstreetmap.org/wiki/Planet.osm/diffs
    //  std::string l_url_diff("http://planet.openstreetmap.org/minute-replicate/");
    //std::string l_url_diff("http://planet.openstreetmap.org/redaction-period/minute-replicate/");
    std::string l_url_diff("http://planet.openstreetmap.org/replication/minute/");
    l_url_diff += l_complete_seq_number.substr(0,3) + "/" + l_complete_seq_number.substr(3,3) + "/" + l_complete_seq_number.substr(6,3) + ".osc.gz";
    return l_url_diff;
  }

  //------------------------------------------------------------------------------
  uint64_t osm_diff_watcher::get_sequence_number(void)const
  {
    std::string l_sequence_number;
    url_reader l_url_reader;
    download_buffer l_buffer;
    l_url_reader.read_url("http://planet.openstreetmap.org/replication/minute/state.txt",l_buffer);
    //  l_url_reader.read_url("http://planet.openstreetmap.org/redaction-period/minute-replicate/state.txt",l_buffer);
    std::stringstream l_stream;
    l_stream << l_buffer.get_data();
    std::string l_line;
    while(!getline(l_stream,l_line).eof())
      {
	if(l_line.find("sequenceNumber") != std::string::npos)
	  {
	    std::cout << "line = \"" << l_line << "\"" << std::endl ;
	    size_t l_begin = l_line.find("=");
	    l_sequence_number = l_line.substr(l_begin+1);
	  }
      }
    assert(l_sequence_number != "");
    return atoll(l_sequence_number.c_str());
  }

}

//EOF
