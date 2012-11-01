#include "osm_ressources.h"
#include "url_reader.h"
#include "download_buffer.h"
#include "osm_diff_state.h"
#include <sstream>
#include <cassert>
#include <iostream>
#include <cstdlib>

using namespace quicky_url_reader;

namespace osm_diff_watcher
{

  osm_ressources::osm_ressources(void):
    m_domain("openstreetmap.org"),
    m_data_domain("planet."+m_domain),
    m_redaction_domain(m_data_domain+"/redaction_period"), //Latest sequence number 229907
    m_replication_domain(m_data_domain+"/replication"),
    m_cc_by_sa_domain(m_data_domain+"cc-by-sa"), // Latest minute sequence number 1268792
    m_api("www."+m_domain+"/api"),
    m_current_api_version("0.6"),
    m_current_api(m_api+"/"+m_current_api_version)
  {
  }

  //"http://planet.openstreetmap.org/minute-replicate/"
  //"http://planet.openstreetmap.org/redaction-period/minute-replicate/"
  //"http://planet.openstreetmap.org/replication/minute/"

  //----------------------------------------------------------------------------
  void osm_ressources::get_user_subscription_date(const std::string & p_user_name,std::string & p_date)const
  {
    std::string l_user = p_user_name;
    std::string l_user_date;
    std::string l_user_url("www."+m_domain+"/user/");
    url_reader l_url_reader;
    char * l_escaped_user = l_url_reader.escape_string(l_user.c_str());
    l_user_url += l_escaped_user;
    download_buffer l_buffer;
    l_url_reader.read_url(l_user_url.c_str(),l_buffer);
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
        std::cout << "WARNING : Unable to find subscription date of user \"" << p_user_name << "\"" << std::endl ;
      }
    p_date = l_user_date;
  }

  //------------------------------------------------------------------------------
  const std::string osm_ressources::get_url_minute_diff(const uint64_t & p_seq_number)const
  {
    std::stringstream l_stream;
    l_stream << p_seq_number;
    std::string l_seq_number = l_stream.str();
    std::string l_complete_seq_number = (l_seq_number.size() < 9 ? std::string(9 - l_seq_number.size(),'0') + l_seq_number : l_seq_number);
    // For more information refer to OSM wiki page
    // http://wiki.openstreetmap.org/wiki/Planet.osm/diffs
    //  std::string l_url_diff("http://planet.openstreetmap.org/minute-replicate/");
    //std::string l_url_diff("http://planet.openstreetmap.org/redaction-period/minute-replicate/");
    std::string l_url_diff(m_replication_domain+"/minute/");
    l_url_diff += l_complete_seq_number.substr(0,3) + "/" + l_complete_seq_number.substr(3,3) + "/" + l_complete_seq_number.substr(6,3) + ".osc.gz";
    return l_url_diff;
  }

  //------------------------------------------------------------------------------
  const osm_diff_analyzer_if::osm_diff_state * osm_ressources::get_minute_diff_state(void)const
  {
    std::string l_sequence_number_str;
    std::string l_timestamp;
    url_reader l_url_reader;
    download_buffer l_buffer;
    l_url_reader.read_url((m_replication_domain+"/minute/state.txt").c_str(),l_buffer);
    //  l_url_reader.read_url("http://planet.openstreetmap.org/redaction-period/minute-replicate/state.txt",l_buffer);
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
            std::cout << "timestamp=\"" << l_timestamp << "\"" << std::endl ;
	  }
      }
    assert(l_sequence_number_str != "");
    assert(l_timestamp != "");
    uint64_t l_sequence_number = atoll(l_sequence_number_str.c_str());
    return new osm_diff_analyzer_if::osm_diff_state(l_sequence_number,l_timestamp);
  }

  //------------------------------------------------------------------------------
  osm_ressources & osm_ressources::instance(void)
  {
    if(m_instance == NULL)
      {
        m_instance = new osm_ressources();
      }
    return *m_instance;
  }

  //------------------------------------------------------------------------------
  void osm_ressources::remove_instance(void)
  {
    delete m_instance;
  }

  osm_ressources * osm_ressources::m_instance = NULL;
  
}
//EOF
