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
#include <limits>
#ifdef _WIN32
#include <windows.h>
#endif
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
#ifndef FORCE_USE_OF_REINTERPRET_CAST
	    osm_diff_analyzer_dom_if::dom_analyzer_if * l_dom_analyzer = dynamic_cast<osm_diff_analyzer_dom_if::dom_analyzer_if *>(l_analyzer);
#else
	    osm_diff_analyzer_dom_if::dom_analyzer_if * l_dom_analyzer = reinterpret_cast<osm_diff_analyzer_dom_if::dom_analyzer_if *>(l_analyzer);
#endif // FORCE_USE_OF_REINTERPRET_CAST
	    if(l_dom_analyzer==NULL)
              {
                std::cout << "Invalid dom_analyzer \"" << l_type << "\"" << std::endl ;
                exit(-1);
               }
	    m_dom_analyzers.insert(make_pair(l_dom_analyzer->get_name(),l_dom_analyzer));
	  }
	else if(l_input_type=="sax")
	  {
#ifndef FORCE_USE_OF_REINTERPRET_CAST
	    osm_diff_analyzer_sax_if::sax_analyzer_if * l_sax_analyzer = dynamic_cast<osm_diff_analyzer_sax_if::sax_analyzer_if *>(l_analyzer);
#else
	    osm_diff_analyzer_sax_if::sax_analyzer_if * l_sax_analyzer = reinterpret_cast<osm_diff_analyzer_sax_if::sax_analyzer_if *>(l_analyzer);
#endif // FORCE_USE_OF_REINTERPRET_CAST
            if(l_sax_analyzer==NULL)
              {
                std::cout << "Invalid sax_analyzer \"" << l_type << "\"" << std::endl ;
                exit(-1);
              }
	    m_sax_analyzers.insert(make_pair(l_sax_analyzer->get_name(),l_sax_analyzer));	    
	  }
	else if(l_input_type=="cpp")
	  {
#ifndef FORCE_USE_OF_REINTERPRET_CAST
	    osm_diff_analyzer_cpp_if::cpp_analyzer_if * l_cpp_analyzer = dynamic_cast<osm_diff_analyzer_cpp_if::cpp_analyzer_if *>(l_analyzer);
#else
	    osm_diff_analyzer_cpp_if::cpp_analyzer_if * l_cpp_analyzer = reinterpret_cast<osm_diff_analyzer_cpp_if::cpp_analyzer_if *>(l_analyzer);
#endif // FORCE_USE_OF_REINTERPRET_CAST
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

    // Manage replication dommain
    std::string l_replication_domain = m_configuration->get_variable("replication_domain");
    if(l_replication_domain != "")
      {
	m_ressources.set_replication_domain(l_replication_domain);
      }

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



#ifndef _WIN32
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
#else
    signal(SIGINT,sig_handler);
#endif
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
  const uint64_t osm_diff_watcher::get_start_sequence_number(const osm_diff_analyzer_if::osm_diff_state &p_diff_state)
  {
    std::string l_start_policy = m_configuration->get_variable("start_policy");
    uint64_t l_result;
    if(l_start_policy=="current" || l_start_policy == "")
      {
	std::cout << "Start Policy : Using current sequence number" << std::endl ;
	l_result = p_diff_state.get_sequence_number();
      }
    else if(l_start_policy == "stored")
      {
        // Replication domain should be set before computing next sequence number to detect domain jumps
        std::string l_stored_replication_domain;
        m_informations.get_latest_replication_domain(l_stored_replication_domain);
        if(l_stored_replication_domain != "")
          {
            std::cout << "Start Policy : Using stored replication domain : " << l_stored_replication_domain << std::endl ;
            m_ressources.set_replication_domain(l_stored_replication_domain);
          }
	l_result = m_informations.get_latest_sequence_number();
	if(!l_result)
	  {
	    std::cout << "WARNING : Using Start policy \"stored\" whereas no sequence number has been stored, start with current sequence" << std::endl;
	    l_result = p_diff_state.get_sequence_number();
	  }
	else
	  {
            std::cout << "Start Policy : Using stored sequence number : " << l_result << std::endl ;
	    l_result = get_next_sequence_number(l_result);
	  }
      }
    else if(l_start_policy == "user_defined")
      {
	std::string l_start_value = m_configuration->get_variable("start_sequence_number");
	if(l_start_value=="")
	  {
	    std::cout << "Start Policy : Using user defined sequence number" << std::endl ;
	    std::cout << "ERROR : \"start_sequence_number\" should be defined when using start_policy \"user_defined\"" << std::endl ;
	    exit(EXIT_FAILURE);
	  }
	l_result = strtoull(l_start_value.c_str(),NULL,10);
      }
    else if(l_start_policy != "")
      {
	std::cout << "ERROR : unexpected value for start_policy : \"" << l_start_policy << "\"" << std::endl ;
	std::cout << "\t Value should be \"current\",\"stored\" or \"user_defined\"" << std::endl ;
	exit(EXIT_FAILURE);
      }

    return l_result;
  }

  //------------------------------------------------------------------------------
  void osm_diff_watcher::run(void)
  {
    const osm_diff_analyzer_if::osm_diff_state * l_diff_state = m_ressources.get_minute_diff_state();
    uint64_t l_end_seq_number = l_diff_state->get_sequence_number();

    // Get start sequence number
    uint64_t l_current_seq_number = get_start_sequence_number(*l_diff_state);

    // Managing iteration number
    uint64_t l_nb_iteration = std::numeric_limits<uint64_t>::max();
    bool l_display_iteration = false;
    if(m_configuration->get_variable("iteration_number") != "")
      {
	l_nb_iteration = strtoull(m_configuration->get_variable("iteration_number").c_str(),NULL,10);
        l_display_iteration = true;
      }
    time_t l_previous_diff_time;
    bool l_first_diff = true;
    while(l_nb_iteration && !m_stop)
      {
	std::cout << "--------------------------------------------------------" << std::endl ;
        if(l_display_iteration)
          {
            std::cout << l_nb_iteration << " remaining iterations" << std::endl ;
          }
	std::cout << "Latest seq number available = " << l_end_seq_number << std::endl ;
	std::cout << "--> Sequence number = \"" << l_current_seq_number << "\"" << std::endl;

	std::string l_state_url;
	m_ressources.get_state_url_diff(l_state_url,l_current_seq_number);

	if(l_current_seq_number != l_end_seq_number)
	  {
	    delete l_diff_state;
            //TO DELETE	    l_diff_state = new osm_diff_analyzer_if::osm_diff_state(l_current_seq_number,"",m_ressources.get_replication_domain());
            l_diff_state = m_ressources.get_minute_diff_state(l_state_url);
	  }
	else if(l_diff_state == NULL)
	  {
            //TO DELETE	    l_diff_state = new osm_diff_analyzer_if::osm_diff_state(l_current_seq_number,"",m_ressources.get_replication_domain());
            l_diff_state = m_ressources.get_minute_diff_state(l_state_url);
	  }
        std::cout << "Timestamp of diff file : " << l_diff_state->get_timestamp() << std::endl ;
        time_t l_diff_time = extract_time(l_diff_state->get_timestamp());

        //Compute time elapsed between two diffs generations to adjust wait delay
        double l_time_between_diff = 60;
        if(!l_first_diff)
          {
            l_time_between_diff = difftime(l_diff_time,l_previous_diff_time); 
          }
        else
          {
            l_first_diff = false;
          }
        l_previous_diff_time = l_diff_time ;

	std::string l_url_diff = m_ressources.get_url_minute_diff(l_current_seq_number);
	std::cout << "Url of diff file \"" << l_url_diff << "\"" << std::endl ;
        time_t l_begin_time = time(NULL);
	bool l_404_error = false;
	
	// Check if download has succeeded
	uint32_t l_404_trial = 3;
	do
	  {
	    m_url_reader.dump_url_binary(l_url_diff,m_diff_file_name);
	    l_404_error = check_404_error(m_diff_file_name);
	    --l_404_trial;
	  } while(l_404_error && l_404_trial);

	if(l_404_error)
	  {
	    std::cout << "ERROR : download of " << l_url_diff << " failed to many times" << std::endl ;
	    exit(-1);
	  }

        // Parse diff file
	parse_diff(l_diff_state);
        
        // Remember reference of parsed diff
	m_informations.store(*l_diff_state);
        delete l_diff_state;
        l_diff_state = NULL;
	--l_nb_iteration;

        // Prepare wait of next diff if necessary : latest available number is current sequence number
	if(l_current_seq_number == l_end_seq_number && !m_stop && l_nb_iteration)
	  {
            uint64_t l_next_seq_number = get_next_sequence_number(l_current_seq_number);

            // Check if we are just before a domain jump. Not necessary to wai tin case of domain jump
            if(l_next_seq_number == l_current_seq_number +1)
              {
                // Determine time needed to run analyzer on diff file
                time_t l_end_time = time(NULL);
                double l_computation_time = difftime(l_end_time,l_begin_time);
                std::cout << "Number of seconds between diffs : " << l_time_between_diff << " s" << std::endl ;
                std::cout << "Time spent in analyze = " << l_computation_time << "s" << std::endl ;

                // Compute remaining time before next diff generation
                double l_remaining_time = 0;
                if(l_computation_time < l_time_between_diff)
                  {
                    l_remaining_time = l_time_between_diff - l_computation_time;
                  }
                // Waiting for new end sequence number
                uint32_t l_delay = (uint32_t) l_remaining_time;
                do
                  {
                    std::cout << "Wait for " << l_delay << " seconds" << std::endl ;
#ifndef _WIN32
                    sleep(l_delay);
#else
                    Sleep(1000*l_delay);
#endif
                    delete l_diff_state;
                    l_diff_state = m_ressources.get_minute_diff_state();
                    l_end_seq_number = l_diff_state->get_sequence_number();
                    // Increase delay in case of unchanged end seqence number
                    if(l_delay == (uint32_t) l_remaining_time) l_delay = 1;
                    if(l_delay < l_time_between_diff / 2) l_delay *= 2;
                  }while(l_current_seq_number == l_end_seq_number && !m_stop);
              
              }
            l_current_seq_number = l_next_seq_number;
 
	  }
        else
          {
            ++l_current_seq_number;
          }
      }
    delete l_diff_state;
    if(m_stop)
      {
        std::cout << "End of run requested by user" << std::endl ;
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
  bool osm_diff_watcher::check_404_error(const std::string & p_file_name)
  {
    std::ifstream l_tmp_input_file(p_file_name.c_str());
    if(l_tmp_input_file==NULL)
      {
	std::cout << "ERROR : Unable to open file \"" << p_file_name << "\"" << std::endl ;
	exit(-1);
      }
    l_tmp_input_file.close();

    igzstream l_input_file(p_file_name.c_str());    
    std::string l_line;
    bool l_eof = getline(l_input_file,l_line).eof();
    bool l_error = l_line.substr(0,std::string("<?xml").size()) != "<?xml" && l_line.substr(0,std::string("<osmChange").size()) != "<osmChange";
    l_input_file.close();
    return l_error || l_eof;
  }

  bool osm_diff_watcher::m_stop = false;
}

//EOF
