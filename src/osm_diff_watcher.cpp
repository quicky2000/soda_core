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
#include "osm_diff_watcher.h"
#include "url_reader.h"
#include "download_buffer.h"
#include "osm_ressources.h"
#include "gzstream.h"
#include "configuration_parser.h"
#include "common_api_wrapper.h"
#include "osm_diff_state.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <signal.h>
using namespace quicky_url_reader;

namespace osm_diff_watcher
{
  //------------------------------------------------------------------------------
  osm_diff_watcher::osm_diff_watcher(const std::string & p_file_name):
    m_ressources(osm_ressources::instance()),
    m_api_wrapper(common_api_wrapper::instance(m_ressources)),
    m_diff_file_name("tmp_diff.gz"),
    m_configuration(NULL),
    m_dom2cpp_analyzer("dom2cpp_analyzer_instance"),
    m_url_reader(quicky_url_reader::url_reader::instance())

  {
    std::string l_file_name = (p_file_name == "" ?  "osm.conf" : p_file_name);
    m_configuration = configuration_parser::parse(l_file_name);
    for(std::vector<std::string>::const_iterator l_iter = m_configuration->get_libraries().begin();
        l_iter != m_configuration->get_libraries().end();
        ++l_iter)
      {
        m_module_manager.load_library(*l_iter);
      }
    for(std::vector<osm_diff_analyzer_if::module_configuration*>::const_iterator l_iter = m_configuration->get_module_configurations().begin();
        l_iter != m_configuration->get_module_configurations().end();
        ++l_iter)
      {
	const osm_diff_analyzer_if::module_configuration * const l_module_configuration = *l_iter;
	std::string l_type = l_module_configuration->get_type();
	bool l_module_enabled = l_module_configuration->is_enabled();
	osm_diff_analyzer_if::diff_analyzer_if * l_analyzer = m_module_manager.create_module<osm_diff_analyzer_if::diff_analyzer_if>(l_module_configuration);
	std::string l_input_type = l_analyzer->get_input_type();
	if(!l_module_enabled)
	  {
	    std::cout << "Module \"" << l_analyzer->get_name() << "\" of type \"" << l_type << "\" has been disable" << std::endl ;
	    m_disabled_analyzers.insert(make_pair(l_analyzer->get_name(),l_analyzer));
	  }
	else if(l_input_type=="dom")
	  {
	    osm_diff_analyzer_dom_if::dom_analyzer_if * l_dom_analyzer = dynamic_cast<osm_diff_analyzer_dom_if::dom_analyzer_if *>(l_analyzer);
	    if(l_dom_analyzer==NULL)
              {
                std::cout << "Invalid dom_analyzer \"" << l_type << "\"" << std::endl ;
                exit(-1);
               }
	    m_dom_analyzers.insert(make_pair(l_dom_analyzer->get_name(),l_dom_analyzer));
	  }
	else if(l_input_type=="sax")
	  {
	    osm_diff_analyzer_sax_if::sax_analyzer_if * l_sax_analyzer = dynamic_cast<osm_diff_analyzer_sax_if::sax_analyzer_if *>(l_analyzer);
            if(l_sax_analyzer==NULL)
              {
                std::cout << "Invalid sax_analyzer \"" << l_type << "\"" << std::endl ;
                exit(-1);
              }
	    m_sax_analyzers.insert(make_pair(l_sax_analyzer->get_name(),l_sax_analyzer));	    
	  }
	else if(l_input_type=="cpp")
	  {
	    osm_diff_analyzer_cpp_if::cpp_analyzer_if * l_cpp_analyzer = dynamic_cast<osm_diff_analyzer_cpp_if::cpp_analyzer_if *>(l_analyzer);
	    if(l_cpp_analyzer==NULL)
              {
                std::cout << "Invalid cpp_analyzer \"" << l_type << "\"" << std::endl ;
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

    // Proxy authentication configuration
    std::string l_proxy_name = m_configuration->get_variable("proxy_authentication.proxy_name");
    std::string l_proxy_port = m_configuration->get_variable("proxy_authentication.proxy_port");
    std::string l_proxy_login = m_configuration->get_variable("proxy_authentication.proxy_login");
    std::string l_proxy_password = m_configuration->get_variable("proxy_authentication.proxy_password");
    if(l_proxy_name != "" &&
       l_proxy_port != "" &&
       l_proxy_login != "" &&
       l_proxy_password != "")
      {
	std::cout << "Configuring proxy authentication" << std::endl ;
	url_reader l_url_reader;
	l_url_reader.set_authentication(l_proxy_name,l_proxy_port,l_proxy_login,l_proxy_password);
      }
    // Number of iteration configuration
    

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

    for(std::map<std::string,osm_diff_analyzer_if::diff_analyzer_if *>::iterator l_iter = m_disabled_analyzers.begin();
        l_iter != m_disabled_analyzers.end();
        ++l_iter)
      {
        delete l_iter->second;
      }
    delete m_configuration;
    common_api_wrapper::remove_instance();
    osm_ressources::remove_instance();
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
    //TO DELETE    uint64_t l_end_seq_number = get_sequence_number();
    const osm_diff_analyzer_if::osm_diff_state * l_diff_state = m_ressources.get_minute_diff_state();
    uint64_t l_end_seq_number = l_diff_state->get_sequence_number();
    uint64_t l_current_seq_number = ( p_start_seq_number ? p_start_seq_number : l_end_seq_number);
    uint32_t l_nb_iteration = 1;//00 ;
    while(l_nb_iteration && !m_stop)
      {
	std::cout << "--------------------------------------------------------" << std::endl ;
	std::cout << l_nb_iteration << " remaining iterations" << std::endl ;
	std::cout << "Latest seq number available = " << l_end_seq_number << std::endl ;
	std::cout << "--> Sequence number = \"" << l_current_seq_number << "\"" << std::endl;
        //TO DELETE	std::string l_url_diff = get_url_diff(l_current_seq_number);
	std::string l_url_diff = m_ressources.get_url_minute_diff(l_current_seq_number);
	std::cout << "Url of diff file \"" << l_url_diff << "\"" << std::endl ;
        time_t l_begin_time = time(NULL);
	m_url_reader.dump_url_binary(l_url_diff,m_diff_file_name);
	parse_diff(l_diff_state);
        delete l_diff_state;
        l_diff_state = NULL;
	--l_nb_iteration;
	if(l_current_seq_number == l_end_seq_number && !m_stop && l_nb_iteration)
	  {
            time_t l_end_time = time(NULL);
            double l_diff_time = difftime(l_end_time,l_begin_time);
	    if(l_diff_time < 60)
	      {
		double l_remaining_time = 60 - l_diff_time;
		std::cout << "Elapsed time = " << l_diff_time << "s" << std::endl ;
		// Waiting for new end sequence number
		uint32_t l_delay = (uint32_t) l_remaining_time;
		do
		  {
		    std::cout << "Wait for " << l_delay << " seconds" << std::endl ;
		    sleep(l_delay);
                    //TO DELETE		    l_end_seq_number = get_sequence_number(); 
                    delete l_diff_state;
                    l_diff_state = m_ressources.get_minute_diff_state();
                    l_end_seq_number = l_diff_state->get_sequence_number();
		    if(l_delay == (uint32_t) l_remaining_time) l_delay = 1;
		    if(l_delay < 30 ) l_delay *= 2;
		  }while(l_current_seq_number == l_end_seq_number && !m_stop);
	      }
	  }
      
	++l_current_seq_number;
      }
    delete l_diff_state;
    if(m_stop)
      {
        std::cout << "End of run requestedd by user" << std::endl ;
      }
  }

  //------------------------------------------------------------------------------
  void osm_diff_watcher::parse_diff(const osm_diff_analyzer_if::osm_diff_state * p_diff_state)
  {
    // Sax analyze
    m_sax_parser.set_diff_state(p_diff_state);
    m_sax_parser.parse(m_diff_file_name);

    // DOM analyze
    m_dom_parser.set_diff_state(p_diff_state);
    m_dom_parser.parse(m_diff_file_name);  
  }

  //------------------------------------------------------------------------------
  //TO DELETE  void osm_diff_watcher::dump_url(const std::string & p_url)
  //TO DELETE  {
  //TO DELETE    url_reader l_url_reader;
  //TO DELETE    download_buffer l_buffer;
  //TO DELETE    l_url_reader.read_url(p_url.c_str(),l_buffer);
  //TO DELETE    std::ofstream l_output_file(m_diff_file_name,std::ios::out | std::ios::binary);
  //TO DELETE    if(l_output_file==NULL)
  //TO DELETE      {
  //TO DELETE	std::cout << "ERROR : Unable to open output file \"tmp_diff.gz\"" << std::endl ;
  //TO DELETE	exit(EXIT_FAILURE);
  //TO DELETE      }
  //TO DELETE    l_output_file.write(l_buffer.get_data(),l_buffer.get_size());
  //TO DELETE    l_output_file.close();
  //TO DELETE  }

  //------------------------------------------------------------------------------
  //TO DELETE  std::string osm_diff_watcher::get_url_diff(const uint64_t & p_seq_number)
  //TO DELETE  {
  //TO DELETE    std::stringstream l_stream;
  //TO DELETE    l_stream << p_seq_number;
  //TO DELETE    std::string l_seq_number = l_stream.str();
  //TO DELETE    std::string l_complete_seq_number = (l_seq_number.size() < 9 ? std::string(9 - l_seq_number.size(),'0') + l_seq_number : l_seq_number);
  //TO DELETE    // For more information refer to OSM wiki page
  //TO DELETE    // http://wiki.openstreetmap.org/wiki/Planet.osm/diffs
  //TO DELETE    //  std::string l_url_diff("http://planet.openstreetmap.org/minute-replicate/");
  //TO DELETE    //std::string l_url_diff("http://planet.openstreetmap.org/redaction-period/minute-replicate/");
  //TO DELETE    std::string l_url_diff("http://planet.openstreetmap.org/replication/minute/");
  //TO DELETE    l_url_diff += l_complete_seq_number.substr(0,3) + "/" + l_complete_seq_number.substr(3,3) + "/" + l_complete_seq_number.substr(6,3) + ".osc.gz";
  //TO DELETE    return l_url_diff;
  //TO DELETE  }

  //------------------------------------------------------------------------------
  //TO DELETE  uint64_t osm_diff_watcher::get_sequence_number(void)const
  //TO DELETE  {
  //TO DELETE    std::string l_sequence_number;
  //TO DELETE    std::string l_timestamp;
  //TO DELETE    url_reader l_url_reader;
  //TO DELETE    download_buffer l_buffer;
  //TO DELETE    l_url_reader.read_url("http://planet.openstreetmap.org/replication/minute/state.txt",l_buffer);
  //TO DELETE    //  l_url_reader.read_url("http://planet.openstreetmap.org/redaction-period/minute-replicate/state.txt",l_buffer);
  //TO DELETE    std::stringstream l_stream;
  //TO DELETE    l_stream << l_buffer.get_data();
  //TO DELETE    std::string l_line;
  //TO DELETE    while(!getline(l_stream,l_line).eof())
  //TO DELETE      {
  //TO DELETE	if(l_line.find("sequenceNumber") != std::string::npos)
  //TO DELETE	  {
  //TO DELETE	    size_t l_begin = l_line.find("=");
  //TO DELETE	    l_sequence_number = l_line.substr(l_begin+1);
  //TO DELETE	  }
  //TO DELETE	if(l_line.find("timestamp=") != std::string::npos)
  //TO DELETE	  {
  //TO DELETE	    size_t l_begin = l_line.find("=");
  //TO DELETE	    l_timestamp = l_line.substr(l_begin+1);
  //TO DELETE            while((l_begin = l_timestamp.find("\\")) != std::string::npos)
  //TO DELETE              {
  //TO DELETE                std::string l_suffix = l_timestamp.substr(l_begin+1);
  //TO DELETE                std::string l_prefix = l_timestamp.substr(0,l_begin);
  //TO DELETE                l_timestamp = l_prefix+l_suffix;
  //TO DELETE              }
  //TO DELETE            std::cout << "timestamp=\"" << l_timestamp << "\"" << std::endl ;
  //TO DELETE	  }
  //TO DELETE      }
  //TO DELETE    assert(l_sequence_number != "");
  //TO DELETE    return atoll(l_sequence_number.c_str());
  //TO DELETE  }

  bool osm_diff_watcher::m_stop = false;
}

//EOF
