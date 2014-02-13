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
#include <limits>
#include <cerrno>

#ifndef _WIN32
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#else
#include <windows.h>
#include <Fcntl.h>
#endif
using namespace quicky_url_reader;

namespace osm_diff_watcher
{
  //------------------------------------------------------------------------------
  osm_diff_watcher::osm_diff_watcher(const std::string & p_file_name, soda_Ui_if & p_Ui):
    m_Ui(p_Ui),
    m_stop(false),
    m_ressources(osm_ressources::instance(m_Ui)),
    m_api_wrapper(common_api_wrapper::instance(m_ressources,m_Ui)),
    m_diff_file_name("tmp_diff.gz"),
    m_configuration(NULL),
    m_module_manager(p_Ui),
    m_dom2cpp_analyzer("dom2cpp_analyzer_instance"),
    m_sax_parser_activated(false),
    m_dom_parser_activated(false),
    m_url_reader(quicky_url_reader::url_reader::instance())
  {
    m_Ui.clear_common_text();
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
            std::stringstream l_stream;
            l_stream << "Module \"" << l_analyzer->get_name() << "\" of type \"" << l_type << "\" has been disable";
            m_Ui.append_common_text(l_stream.str());
	    m_disabled_analyzers.insert(make_pair(l_analyzer->get_name(),l_analyzer));
	  }
	else if(l_input_type=="dom")
	  {
#ifndef FORCE_USE_OF_REINTERPRET_CAST
	    osm_diff_analyzer_dom_if::dom_analyzer_if * l_dom_analyzer = dynamic_cast<osm_diff_analyzer_dom_if::dom_analyzer_if *>(l_analyzer) != NULL ?  dynamic_cast<osm_diff_analyzer_dom_if::dom_analyzer_if *>(l_analyzer) : reinterpret_cast<osm_diff_analyzer_dom_if::dom_analyzer_if *>(l_analyzer);
#else
	    osm_diff_analyzer_dom_if::dom_analyzer_if * l_dom_analyzer = reinterpret_cast<osm_diff_analyzer_dom_if::dom_analyzer_if *>(l_analyzer);
#endif // FORCE_USE_OF_REINTERPRET_CAST
	    if(l_dom_analyzer==NULL)
              {
		std::stringstream l_stream;
                l_stream << "Invalid dom_analyzer \"" << l_type << "\""  ;
                throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
              }
	    m_dom_analyzers.insert(make_pair(l_dom_analyzer->get_name(),l_dom_analyzer));
	  }
	else if(l_input_type=="sax")
	  {
#ifndef FORCE_USE_OF_REINTERPRET_CAST
	    osm_diff_analyzer_sax_if::sax_analyzer_if * l_sax_analyzer = dynamic_cast<osm_diff_analyzer_sax_if::sax_analyzer_if *>(l_analyzer) != NULL ? dynamic_cast<osm_diff_analyzer_sax_if::sax_analyzer_if *>(l_analyzer) : reinterpret_cast<osm_diff_analyzer_sax_if::sax_analyzer_if *>(l_analyzer);
#else
	    osm_diff_analyzer_sax_if::sax_analyzer_if * l_sax_analyzer = reinterpret_cast<osm_diff_analyzer_sax_if::sax_analyzer_if *>(l_analyzer);
#endif // FORCE_USE_OF_REINTERPRET_CAST
            if(l_sax_analyzer==NULL)
              {
		std::stringstream l_stream;
                l_stream << "Invalid sax_analyzer \"" << l_type << "\""  ;
                throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
              }
            m_sax_analyzers.insert(make_pair(l_sax_analyzer->get_name(),l_sax_analyzer));
	  }
	else if(l_input_type=="cpp")
	  {
#ifndef FORCE_USE_OF_REINTERPRET_CAST
	    osm_diff_analyzer_cpp_if::cpp_analyzer_if * l_cpp_analyzer = dynamic_cast<osm_diff_analyzer_cpp_if::cpp_analyzer_if *>(l_analyzer) != NULL ? dynamic_cast<osm_diff_analyzer_cpp_if::cpp_analyzer_if *>(l_analyzer) :  reinterpret_cast<osm_diff_analyzer_cpp_if::cpp_analyzer_if *>(l_analyzer);
#else
	    osm_diff_analyzer_cpp_if::cpp_analyzer_if * l_cpp_analyzer = reinterpret_cast<osm_diff_analyzer_cpp_if::cpp_analyzer_if *>(l_analyzer);
#endif // FORCE_USE_OF_REINTERPRET_CAST
	    if(l_cpp_analyzer==NULL)
              {
		std::stringstream l_stream;
                l_stream << "Invalid cpp_analyzer \"" << l_type << "\""  ;
                throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
              }
	    m_cpp_analyzers.insert(make_pair(l_cpp_analyzer->get_name(),l_cpp_analyzer));	    
	  }
	else
	  {
	    std::stringstream l_stream;
	    l_stream << "Unknown input type \"" << l_input_type << "\""  ;
	    throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
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
        m_Ui.append_common_text("Configuring proxy authentication");
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
	m_sax_parser_activated = true;
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
	m_dom_parser_activated = true;
      }
    // Add built-in DOM parsers
    if(m_dom2cpp_analyzer.get_nb_analyzers())
      {
	m_Ui.append_common_text("Activating dom2cpp analzyer");
	m_dom_parser.add_analyzer(m_dom2cpp_analyzer);
	m_dom_parser_activated = true;
      }
    if(m_sax_parser_activated)
      {
	m_Ui.append_common_text("Activating sax analyzer");
      }
    if(m_dom_parser_activated)
      {
	m_Ui.append_common_text("Activating dom analyzer");
      }
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
  const uint64_t osm_diff_watcher::get_start_sequence_number(const osm_diff_analyzer_if::osm_diff_state &p_diff_state)
  {
    std::string l_start_policy = m_configuration->get_variable("start_policy");
    uint64_t l_result=0;
    if(l_start_policy=="current" || l_start_policy == "")
      {
        m_Ui.append_common_text("Start Policy : Using current sequence number");
	l_result = p_diff_state.get_sequence_number();
      }
    else if(l_start_policy == "stored")
      {
        // Replication domain should be set before computing next sequence number to detect domain jumps
        std::string l_stored_replication_domain;
        m_informations.get_latest_replication_domain(l_stored_replication_domain);
        if(l_stored_replication_domain != "")
          {
            std::stringstream l_stream;
            l_stream << "Start Policy : Using stored replication domain : " << l_stored_replication_domain ;
            m_Ui.append_common_text(l_stream.str());
            m_ressources.set_replication_domain(l_stored_replication_domain);
          }
	l_result = m_informations.get_latest_sequence_number();
	if(!l_result)
	  {
            m_Ui.append_common_text("WARNING : Using Start policy \"stored\" whereas no sequence number has been stored, start with current sequence");
	    l_result = p_diff_state.get_sequence_number();
	  }
	else
	  {
            std::stringstream l_stream;
            l_stream << "Start Policy : Using stored sequence number : " << l_result ;
            m_Ui.append_common_text(l_stream.str());
	    l_result = get_next_sequence_number(l_result);
          }
      }
    else if(l_start_policy == "user_defined")
      {
	std::string l_start_value = m_configuration->get_variable("start_sequence_number");
	if(l_start_value=="")
	  {
	    std::stringstream l_stream;
	    l_stream << "ERROR : \"start_sequence_number\" should be defined when using start_policy \"user_defined\""  ;
	    throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
	  }
	l_result = strtoull(l_start_value.c_str(),NULL,10);
      }
    else if(l_start_policy != "")
      {
	std::stringstream l_stream;
	l_stream << "ERROR : unexpected value for start_policy : \"" << l_start_policy << "\".";
	l_stream << "Value should be \"current\",\"stored\" or \"user_defined\""  ;
	throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
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
    uint64_t l_configured_iteration = 0;
    bool l_display_iteration = false;
    if(m_configuration->get_variable("iteration_number") != "")
      {
	l_nb_iteration = strtoull(m_configuration->get_variable("iteration_number").c_str(),NULL,10);
        l_configured_iteration = l_nb_iteration;
        m_Ui.configure_progress_display(0,l_nb_iteration);
        l_display_iteration = true;
      }
    time_t l_previous_diff_time=0;
    bool l_first_diff = true;
 
    double l_remaining_time = 0;
    double l_time_between_diff = 0;
    while(l_nb_iteration && !m_stop)
      {
        m_Ui.append_common_text("--------------------------------------------------------");
        if(l_display_iteration)
          {
            m_Ui.update_progress(l_configured_iteration - l_nb_iteration +1);
            std::stringstream l_stream;
            l_stream << l_nb_iteration << " remaining iterations";
            m_Ui.append_common_text(l_stream.str());
          }
        {
	  std::stringstream l_stream;
	  l_stream << "Latest seq number that was available = " << l_end_seq_number;
	  m_Ui.append_common_text(l_stream.str());
        }
        // Waiting for end sequence number
        uint32_t l_delay = (uint32_t) l_remaining_time;
        // In case of replication domain jump it is important to recompute end_seq_time so 
        // using do while ensure that it will be recomputed at least once
        bool l_first_loop = true;
        do
          {
            if(!l_first_loop && l_delay)
              {
                {
		  std::stringstream l_stream;
		  l_stream << "Wait for " << l_delay << " seconds";
		  m_Ui.append_common_text(l_stream.str());
                }
#ifndef _WIN32
                sleep(l_delay);
#else
                Sleep(1000*l_delay);
#endif
              }
            delete l_diff_state;
            l_diff_state = m_ressources.get_minute_diff_state();
            l_end_seq_number = l_diff_state->get_sequence_number();
            
            if(!l_first_loop)
              {
                // Increase delay in case of unchanged end seqence number
                if(l_delay == (uint32_t) l_remaining_time) l_delay = 1;
                if(l_delay < l_time_between_diff / 2 || l_time_between_diff == 0) l_delay *= 2;
              }
            l_first_loop = false;
          }while(l_current_seq_number > l_end_seq_number && !m_stop);

        if(!m_stop)
          {
            if(l_current_seq_number != l_end_seq_number)
              {
                delete l_diff_state;
                std::string l_state_url;
                m_ressources.get_state_url_diff(l_state_url,l_current_seq_number);
                l_diff_state = m_ressources.get_minute_diff_state(l_state_url);
              }

            time_t l_diff_time = extract_time(l_diff_state->get_timestamp());

            //Compute time elapsed between two diffs generations to adjust wait delay
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

            {
	      std::stringstream l_stream;
	      l_stream << "Url of diff file <A HREF=\"" << l_url_diff << "\">" << l_url_diff << "</A>" ;
	      m_Ui.append_common_text(l_stream.str());
            }
            time_t l_begin_time = time(NULL);
            bool l_404_error = false;
	
	    // Obtain File descriptor
#ifdef _WIN32
	    HANDLE l_file_handle = CreateFile("toto",GENERIC_READ | GENERIC_WRITE,0,0,OPEN_ALWAYS,/*FILE_ATTRIBUTE_NORMAL*/FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE,NULL);
	    if(INVALID_HANDLE_VALUE == l_file_handle)
	      {
		throw quicky_exception::quicky_logic_exception("Problem when opening temp file for diff download",__LINE__,__FILE__);
	      }
	    
	    // Get the file descriptor from the handle
	    int l_fd = _open_osfhandle((long)l_file_handle, O_RDONLY);
#else
	    //SHARED MEM	    int l_fd = shm_open("/soda_sharedmem",O_RDWR|O_CREAT,0);
	    int l_fd = open("tmp_diff.osc.gz",O_RDWR|O_CREAT,0);
	    if(l_fd<0) 
	      {
                std::stringstream l_stream;
                l_stream <<  "Unable to obtain shared mem for diff download : " << strerror(errno) << std::endl << "Please try to restart";
		shm_unlink("/soda_sharedmem");
		throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
	      }
	    ;
#endif
	    try
	      {

		// Check if download has succeeded
		uint32_t l_404_trial = 3;
		
		do
		  {
		    // Start at the beginning of the file
		    lseek(l_fd,0,SEEK_SET);
		    
		    m_url_reader.dump_url(l_url_diff,l_fd);
		    l_404_error = check_404_error(l_fd);
		    --l_404_trial;
		  } while(l_404_error && l_404_trial);
		
		if(l_404_error)
		  {
		    std::stringstream l_stream;
		    l_stream << "ERROR : content downloaded from " << l_url_diff << " doesn't start as expected";
		    throw quicky_exception::quicky_runtime_exception(l_stream.str(),__LINE__,__FILE__);
		  }
		
		// Send diff state to Ui
		m_Ui.update_diff_state(*l_diff_state);
		
		// Parse diff file
		parse_diff(*l_diff_state,l_fd);
	      }
	    catch(quicky_exception::quicky_runtime_exception e)
	      {
		// Close file descriptor
#ifdef _WIN32
		CloseHandle(l_file_handle);
#else
		close(l_fd);
		//SHARED MEMshm_unlink("/soda_sharedmem");
#endif
		throw e;
	      }
	    catch(quicky_exception::quicky_logic_exception e)
	      {
		// Close file descriptor
#ifdef _WIN32
		CloseHandle(l_file_handle);
#else
		close(l_fd);
		//SHARED MEMshm_unlink("/soda_sharedmem");
#endif
		throw e;
	      }
	    catch(std::exception e)
	      {
		// Close file descriptor
#ifdef _WIN32
		CloseHandle(l_file_handle);
#else
		close(l_fd);
		//SHARED MEMshm_unlink("/soda_sharedmem");
#endif
		throw e;
	      }

	    // Close file descriptor
#ifdef _WIN32
	    CloseHandle(l_file_handle);
#else
	    close(l_fd);
	    //SHARED MEMshm_unlink("/soda_sharedmem");
#endif
        
	    
            // Remember reference of parsed diff
            m_informations.store(*l_diff_state);
            --l_nb_iteration;

            // Prepare dely to wait next diff if necessary : latest available number is current sequence number and we are not at domain jump
            l_remaining_time = 0;
            uint64_t l_next_seq_number = get_next_sequence_number(l_current_seq_number);

            if(l_current_seq_number == l_end_seq_number && !m_stop && l_nb_iteration)
              {
                // Check if we are just before a domain jump. Not necessary to wait in case of domain jump
                if(l_next_seq_number == l_current_seq_number +1)
                  {
                    // Determine time needed to run analyzer on diff file
                    time_t l_end_time = time(NULL);
                    double l_computation_time = difftime(l_end_time,l_begin_time);

                    {
		      std::stringstream l_stream;
		      l_stream << "Number of seconds between diffs : " << l_time_between_diff << " s" ;
		      m_Ui.append_common_text(l_stream.str());
                    }
                    {
		      std::stringstream l_stream;
		      l_stream << "Time spent in analyze = " << l_computation_time << "s" ;
		      m_Ui.append_common_text(l_stream.str());
                    }

                    // Compute remaining time before next diff generation
                    if(l_computation_time < l_time_between_diff)
                      {
                        l_remaining_time = l_time_between_diff - l_computation_time;
                      }
                  } 
              }
            l_current_seq_number = l_next_seq_number;
          }
      }
    delete l_diff_state;
    if(m_stop)
      {
        m_Ui.append_common_text("End of run requested by user");
      }
  }

  //------------------------------------------------------------------------------
  void osm_diff_watcher::parse_diff(const osm_diff_analyzer_if::osm_diff_state & p_diff_state,const int & p_fd)
  {
    // Sax analyze
    if(m_sax_parser_activated)
      {
	m_sax_parser.set_diff_state(p_diff_state);
	int l_fd_cpy = dup(p_fd);
	m_sax_parser.parse(l_fd_cpy);
      }

    // DOM analyze
    if(m_dom_parser_activated)
      {
	m_dom_parser.set_diff_state(p_diff_state);
	int l_fd_cpy = dup(p_fd);
	m_dom_parser.parse(l_fd_cpy);  
      }
  }
  
  //------------------------------------------------------------------------------
  bool osm_diff_watcher::check_404_error(const int & p_fd)
  {
    int l_fp_cpy = dup(p_fd);
    lseek(l_fp_cpy,0,SEEK_SET);
    uint8_t l_buffer_ref[] = { 0x1f,0x8b};
    uint8_t l_buffer_ref2[] = {  '<','o','s','m','C','h','a','n','g','e'};
    uint8_t l_buffer_check[] = { 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
    int l_result = read(l_fp_cpy,&l_buffer_check,10);
    bool l_error = l_result != 10;
    if(!l_error)
      {
	l_error = memcmp(&l_buffer_ref,&l_buffer_check,2) &&  memcmp(&l_buffer_ref2,&l_buffer_check,10) ;
      }
    close(l_fp_cpy);
    return l_error;
  }

}

//EOF
