#include "osm_ressources.h"
#include "url_reader.h"
#include "download_buffer.h"
#include <sstream>
#include <cassert>
#include <iostream>

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
        std::cout << "WARNING : Unable to find subscruption date of user \"" << p_user_name << "\"" << std::endl ;
      }
    p_date = l_user_date;
  }

}
//EOF
