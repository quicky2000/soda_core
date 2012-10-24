#include "osm_diff_watcher.h"
#include "url_reader.h"
#include "download_buffer.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <stdlib.h>
#include "gzstream.h"
#include "configuration_parser.h"
#include <signal.h>
using namespace quicky_url_reader;

namespace osm_diff_watcher
{
  //------------------------------------------------------------------------------
  osm_diff_watcher::osm_diff_watcher(const std::string & p_file_name):
    m_dom2cpp_analyzer("dom2cpp_analyzer_instance")
  {
    std::string l_file_name = (p_file_name == "" ?  "osm.conf" : p_file_name);
    const configuration * l_configuration = configuration_parser::parse(l_file_name);
    for(std::vector<std::string>::const_iterator l_iter = l_configuration->get_libraries().begin();
        l_iter != l_configuration->get_libraries().end();
        ++l_iter)
      {
        m_module_manager.load_library(*l_iter);
      }
    for(std::vector<std::pair<std::string,std::string> >::const_iterator l_iter = l_configuration->get_modules().begin();
        l_iter != l_configuration->get_modules().end();
        ++l_iter)
      {
	osm_diff_analyzer_if::general_analyzer_if * l_analyzer = m_module_manager.create_module<osm_diff_analyzer_if::general_analyzer_if>(l_iter->second,l_iter->first);
	std::string l_input_type = l_analyzer->get_input_type();
	if(l_input_type=="dom")
	  {
	    osm_diff_analyzer_dom_if::dom_analyzer_if * l_dom_analyzer = dynamic_cast<osm_diff_analyzer_dom_if::dom_analyzer_if *>(l_analyzer);
	    if(l_dom_analyzer==NULL)
              {
                std::cout << "Invalid dom_analyzer \"" << l_iter->second << "\"" << std::endl ;
                exit(-1);
               }
	    m_dom_analyzers.insert(make_pair(l_dom_analyzer->get_name(),l_dom_analyzer));
	  }
	else if(l_input_type=="sax")
	  {
	    osm_diff_analyzer_sax_if::sax_analyzer_if * l_sax_analyzer = dynamic_cast<osm_diff_analyzer_sax_if::sax_analyzer_if *>(l_analyzer);
            if(l_sax_analyzer==NULL)
              {
                std::cout << "Invalid sax_analyzer \"" << l_iter->second << "\"" << std::endl ;
                exit(-1);
              }
	    m_sax_analyzers.insert(make_pair(l_sax_analyzer->get_name(),l_sax_analyzer));	    
	  }
	else if(l_input_type=="cpp")
	  {
	    osm_diff_analyzer_cpp_if::cpp_analyzer_if * l_cpp_analyzer = dynamic_cast<osm_diff_analyzer_cpp_if::cpp_analyzer_if *>(l_analyzer);
	    if(l_cpp_analyzer==NULL)
              {
                std::cout << "Invalid cpp_analyzer \"" << l_iter->second << "\"" << std::endl ;
                exit(-1);
              }
	    m_cpp_analyzers.insert(make_pair(l_cpp_analyzer->get_name(),l_cpp_analyzer));	    
	  }
	else
	  {
	    std::cout << "Unknown input type \"" << l_input_type << "\"" << std::endl ;
	    exit(-1);
	  }
      }

    std::string l_proxy_name = l_configuration->get_variable("proxy_authentication.proxy_name");
    std::string l_proxy_port = l_configuration->get_variable("proxy_authentication.proxy_port");
    std::string l_proxy_login = l_configuration->get_variable("proxy_authentication.proxy_login");
    std::string l_proxy_password = l_configuration->get_variable("proxy_authentication.proxy_password");
    if(l_proxy_name != "" &&
       l_proxy_port != "" &&
       l_proxy_login != "" &&
       l_proxy_password != "")
      {
	std::cout << "Configuring proxy authentication" << std::endl ;
	url_reader l_url_reader;
	l_url_reader.set_authentication(l_proxy_name,l_proxy_port,l_proxy_login,l_proxy_password);
      }
    delete l_configuration;

    // Add loaded SAX parsers
    for(std::map<std::string,osm_diff_analyzer_sax_if::sax_analyzer_if *>::iterator l_iter = m_sax_analyzers.begin();
        l_iter != m_sax_analyzers.end();
        ++l_iter)
      {
        m_sax_parser.add_analyzer(*(l_iter->second));
      }

    // Add built-in SAX parsers

    // Add loaded CPP parsers
    for(std::map<std::string,osm_diff_analyzer_cpp_if::cpp_analyzer_if *>::iterator l_iter = m_cpp_analyzers.begin();
        l_iter != m_cpp_analyzers.end();
        ++l_iter)
      {
        m_dom2cpp_analyzer.add_analyzer(*(l_iter->second));
      }

    // Add built-in CPP parsers

    // Add loaded DOM parsers
    for(std::map<std::string,osm_diff_analyzer_dom_if::dom_analyzer_if *>::iterator l_iter = m_dom_analyzers.begin();
        l_iter != m_dom_analyzers.end();
        ++l_iter)
      {
        m_dom_parser.add_analyzer(*(l_iter->second));
      }
    // Add built-in DOM parsers
    m_dom_parser.add_analyzer(m_dom2cpp_analyzer);


    //Preparing signal handling to manage stop
    /* Déclaration d'une structure pour la mise en place des gestionnaires */
    struct sigaction l_signal_action;
  
    /* Remplissage de la structure pour la mise en place des gestionnaires */
    /* adresse du gestionnaire */
    l_signal_action.sa_handler=sig_handler;
    // Mise a zero du champ sa_flags theoriquement ignoré
    l_signal_action.sa_flags=0;
    /* On ne bloque pas de signaux spécifiques */
    sigemptyset(&l_signal_action.sa_mask);
    
    /* Mise en place du gestionnaire bidon pour trois signaux */
    sigaction(SIGINT,&l_signal_action,0);
  }

  //------------------------------------------------------------------------------
  osm_diff_watcher::~osm_diff_watcher(void)
  {
    for(std::map<std::string,osm_diff_analyzer_dom_if::dom_analyzer_if *>::iterator l_iter = m_dom_analyzers.begin();
        l_iter != m_dom_analyzers.end();
        ++l_iter)
      {
        delete l_iter->second;
      }

    for(std::map<std::string,osm_diff_analyzer_sax_if::sax_analyzer_if *>::iterator l_iter = m_sax_analyzers.begin();
        l_iter != m_sax_analyzers.end();
        ++l_iter)
      {
        delete l_iter->second;
      }

    for(std::map<std::string,osm_diff_analyzer_cpp_if::cpp_analyzer_if *>::iterator l_iter = m_cpp_analyzers.begin();
        l_iter != m_cpp_analyzers.end();
        ++l_iter)
      {
        delete l_iter->second;
      }

  }

  //------------------------------------------------------------------------------
  void osm_diff_watcher::sig_handler(int p_sig)
  {
    std::cout << "===================> Receive Control-C : execution will stop after this minute diff analyze will be finished" << std::endl ;
    m_stop = true;
  }

  //------------------------------------------------------------------------------
  void osm_diff_watcher::run(const uint64_t & p_start_seq_number)
  {
    uint64_t l_end_seq_number = get_sequence_number();
    uint64_t l_current_seq_number = ( p_start_seq_number ? p_start_seq_number : l_end_seq_number);
    uint32_t l_nb_iteration = 3;//00 ;
    while(l_nb_iteration && !m_stop)
      {
	std::cout << "--------------------------------------------------------" << std::endl ;
	std::cout << l_nb_iteration << " remaining iterations" << std::endl ;
	std::cout << "Latest seq number available = " << l_end_seq_number << std::endl ;
	std::cout << "--> Sequence number = \"" << l_current_seq_number << "\"" << std::endl;
	std::string l_url_diff = get_url_diff(l_current_seq_number);
	std::cout << "Url of diff file \"" << l_url_diff << "\"" << std::endl ;
        time_t l_begin_time = time(NULL);
	dump_url(l_url_diff);
	parse_diff();
	if(l_current_seq_number == l_end_seq_number)
	  {
            time_t l_end_time = time(NULL);
            double l_diff_time = difftime(l_end_time,l_begin_time);
            double l_remaining_time = 60 - l_diff_time;
            std::cout << "Elapsed time = " << l_diff_time << "s" << std::endl ;
	    // Waiting for new end sequence number
	    uint32_t l_delay = (uint32_t) l_remaining_time;
	    do
	      {
		std::cout << "Wait for " << l_delay << " seconds" << std::endl ;
		sleep(l_delay);
		l_end_seq_number = get_sequence_number();   
                if(l_delay == (uint32_t) l_remaining_time) l_delay = 1;
		if(l_delay < 30 ) l_delay *= 2;
	      }while(l_current_seq_number == l_end_seq_number && !m_stop);
	  }
      
	++l_current_seq_number;
	--l_nb_iteration;
      }
    if(m_stop)
      {
        std::cout << "End of run requestedd by user" << std::endl ;
      }
  }

  //------------------------------------------------------------------------------
  void osm_diff_watcher::parse_diff(void)
  {
    // Sax analyze
    m_sax_parser.parse("tmp_diff.gz");

    // DOM analyze
    m_dom_parser.parse("tmp_diff.gz");  
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

  bool osm_diff_watcher::m_stop = false;
}

//EOF
