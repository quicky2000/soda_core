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
#include "osm_ressources.h"
#include "url_reader.h"
#include "download_buffer.h"
#include "osm_diff_state.h"
#include "dom_parser.h"
#include "dom_osm_change_extractor.h"
#include "dom_osm_full_extractor.h"
#include "quicky_exception.h"
#include "soda_Ui_if.h"
#include <sstream>
#include <cassert>
#include <iostream>
#include <cstdlib>

using namespace quicky_url_reader;

namespace osm_diff_watcher
{

  osm_ressources::osm_ressources(soda_Ui_if & p_ui):
    m_ui(p_ui),
    m_domain("openstreetmap.org"),
    m_data_domain("https://planet."+m_domain),
    m_redaction_domain(m_data_domain+"/redaction_period"), //Latest sequence number 229907
    m_replication_domain(m_data_domain+"/replication/minute"),
    m_cc_by_sa_domain(m_data_domain+"cc-by-sa"), // Latest minute sequence number 1268792
    m_browse_domain("https://www."+m_domain+"/browse"),
    m_user_url("https://www."+m_domain+"/user"),
    m_api("https://www."+m_domain+"/api"),
    m_current_api_version("0.6"),
    m_current_api(m_api+"/"+m_current_api_version)
  {
  }

  //"https://planet.openstreetmap.org/minute-replicate/"
  //"https://planet.openstreetmap.org/redaction-period/minute-replicate/"
  //"https://planet.openstreetmap.org/replication/minute/"

  //----------------------------------------------------------------------------
  void osm_ressources::get_user_subscription_date(const std::string & p_user_name,std::string & p_date)const
  {
    std::string l_user = p_user_name;
    std::string l_user_date;
    url_reader l_url_reader;
    char * l_escaped_user = l_url_reader.escape_string(l_user.c_str());
    download_buffer l_buffer;
    l_url_reader.read_url((m_user_url +"/"+l_escaped_user).c_str(),l_buffer);
    curl_free(l_escaped_user);
    
    std::stringstream l_stream;
    l_stream << l_buffer.get_data();
    std::string l_line;
    bool l_found = false;
    while(!l_found && !getline(l_stream,l_line).eof())
      {
	size_t l_pos ;
	// expecting line like "Mapper since: August 13, 2009"
	if((l_pos = l_line.find("Mapper since:")) != std::string::npos)
	  {
	    size_t l_begin = l_line.find(":",l_pos);
	    l_begin = l_line.find_first_not_of(" ",l_begin+1);
	    l_user_date = l_line.substr(l_begin);
	    l_found = true;
	  }
      }
    if(!l_found)
      {
	std::stringstream l_stream;
        l_stream << "WARNING : Unable to find subscription date of user \"" << p_user_name << "\"" ;
	m_ui.append_common_text(l_stream.str());
      }
    p_date = l_user_date;
  }

  //------------------------------------------------------------------------------
  const std::string osm_ressources::get_url_minute_diff(const uint64_t & p_seq_number)const
  {
    std::string l_url_diff ;
    get_root_url_diff(l_url_diff,p_seq_number);
    l_url_diff += ".osc.gz";
    return l_url_diff;
  }

  //------------------------------------------------------------------------------
  void osm_ressources::get_root_url_diff(std::string & p_result,const uint64_t & p_seq_number)const
  {
    std::stringstream l_stream;
    l_stream << p_seq_number;
    std::string l_seq_number = l_stream.str();
    std::string l_complete_seq_number = (l_seq_number.size() < 9 ? std::string(9 - l_seq_number.size(),'0') + l_seq_number : l_seq_number);
    p_result = m_replication_domain + "/" + l_complete_seq_number.substr(0,3) + "/" + l_complete_seq_number.substr(3,3) + "/" + l_complete_seq_number.substr(6,3);
  }

  //------------------------------------------------------------------------------
  void osm_ressources::get_state_url_diff(std::string & p_result,const uint64_t & p_seq_number)const
  {
    get_root_url_diff(p_result,p_seq_number);
    p_result += ".state.txt";    
  }

  //------------------------------------------------------------------------------
  const osm_diff_analyzer_if::osm_diff_state * osm_ressources::get_minute_diff_state(const std::string & p_url)const
  {
    std::string l_sequence_number_str;
    std::string l_timestamp;
    uint32_t l_nb_trial = 3;
    std::string l_url = (p_url != "" ? p_url : (m_replication_domain+"/state.txt")); 
    do
      {
	url_reader l_url_reader;
	download_buffer l_buffer;
	l_url_reader.read_url(l_url.c_str(),l_buffer);
	//  l_url_reader.read_url("https://planet.openstreetmap.org/redaction-period/minute-replicate/state.txt",l_buffer);
	std::stringstream l_stream;
	l_stream << l_buffer.get_data();
	std::string l_line;
	while(!getline(l_stream,l_line).eof())
	  {
	    if(l_line.find("sequenceNumber") != std::string::npos)
	      {
		size_t l_begin = l_line.find("=");
		l_sequence_number_str = l_line.substr(l_begin+1);
	      }
	    if(l_line.find("timestamp=") != std::string::npos)
	      {
		size_t l_begin = l_line.find("=");
		l_timestamp = l_line.substr(l_begin+1);
		while((l_begin = l_timestamp.find("\\")) != std::string::npos)
		  {
		    std::string l_suffix = l_timestamp.substr(l_begin+1);
		    std::string l_prefix = l_timestamp.substr(0,l_begin);
		    l_timestamp = l_prefix+l_suffix;
		  }
	      }
	  }
	--l_nb_trial;
      }while((l_sequence_number_str == "" || l_timestamp == "") && l_nb_trial);
    if(l_sequence_number_str == "" || l_timestamp == "")
      {
	std::stringstream l_stream;
	l_stream << "ERROR : too many unsuccessfull attempt to obtain diff information from " << l_url  ;
	throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
      }
    uint64_t l_sequence_number = atoll(l_sequence_number_str.c_str());
    return new osm_diff_analyzer_if::osm_diff_state(l_sequence_number,l_timestamp,m_replication_domain);
  }

  //------------------------------------------------------------------------------
  osm_ressources & osm_ressources::instance(soda_Ui_if & p_ui)
  {
    if(m_instance == NULL)
      {
        m_instance = new osm_ressources(p_ui);
      }
    return *m_instance;
  }

  //------------------------------------------------------------------------------
  void osm_ressources::remove_instance(void)
  {
    delete m_instance;
    m_instance = NULL;
  }

  //------------------------------------------------------------------------------
  const std::vector<osm_api_data_types::osm_change*> * const osm_ressources::get_osm_change_file_content(const std::string & p_file_name)
  {
    dom_parser l_osm_change_parser("osmChange");
    dom_osm_change_extractor l_extractor;
    l_osm_change_parser.add_analyzer(l_extractor);
    l_osm_change_parser.parse(p_file_name);
    return l_extractor.get_result();           
  }

  //------------------------------------------------------------------------------
  void osm_ressources::get_osm_file_content(const std::string & p_file_name,
					    std::vector<osm_api_data_types::osm_node*> & p_nodes,
					    std::vector<osm_api_data_types::osm_way*> & p_ways,
					    std::vector<osm_api_data_types::osm_relation*> & p_relations)
  {
    dom_parser l_dom_parser("osm");
    dom_osm_full_extractor l_extractor(p_nodes,p_ways,p_relations);
    l_dom_parser.add_analyzer(l_extractor);
    l_dom_parser.parse(p_file_name);
  }
  
  osm_ressources * osm_ressources::m_instance = NULL;
  
}
//EOF
