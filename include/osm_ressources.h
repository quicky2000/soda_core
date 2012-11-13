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
#ifndef _OSM_RESSOURCES_H_
#define _OSM_RESSOURCES_H_

#include <string>
#include "url_reader.h"
#include "download_buffer.h"
#include "osm_api_data_types.h"
#include <sstream>

namespace osm_diff_analyzer_if
{
  class osm_diff_state;
}

namespace osm_diff_watcher
{

  class osm_ressources
  {
  public:
    inline const std::string & get_api_url(void)const;
    void get_user_subscription_date(const std::string & p_user_name,std::string & p_date)const;
    const std::string get_url_minute_diff(const uint64_t & p_seq_number)const;
    void get_root_url_diff(std::string & p_result,const uint64_t & p_seq_number)const;
    void get_state_url_diff(std::string & p_result,const uint64_t & p_seq_number)const;
    const osm_diff_analyzer_if::osm_diff_state * get_minute_diff_state(const std::string & p_url="")const;
    static osm_ressources & instance(void);
    static void remove_instance(void);
    inline void set_replication_domain(const std::string & p_replication_domain);
    inline const std::string & get_replication_domain(void)const;
    inline void get_user_browse_url(std::string & p_result,
                                                 const std::string & p_user_name)const;
    inline void get_object_browse_url(std::string & p_result,
                                                   const std::string & p_type,const osm_api_data_types::osm_object::t_osm_id & p_id);
    inline void get_api_object_url(std::string & p_result,
                                                const std::string & p_type,const osm_api_data_types::osm_object::t_osm_id & p_id, const osm_api_data_types::osm_core_element::t_osm_version & p_version=0);
    static const std::vector<osm_api_data_types::osm_change*> * const get_osm_change_file_content(const std::string & p_file_name);
    static void get_osm_file_content(const std::string & p_file_name,
                                     std::vector<osm_api_data_types::osm_node*> & p_nodes,
                                     std::vector<osm_api_data_types::osm_way*> & p_ways,
                                     std::vector<osm_api_data_types::osm_relation*> & p_relations);
  private:
    osm_ressources(void);

    const std::string m_domain;
    const std::string m_data_domain;
    const std::string m_redaction_domain;
    std::string m_replication_domain;
    const std::string m_cc_by_sa_domain;
    const std::string m_browse_domain;
    const std::string m_user_url;
    const std::string m_api;
    const std::string m_current_api_version;
    const std::string m_current_api;
    static osm_ressources * m_instance;
  }
  ;

  //----------------------------------------------------------------------------
  const std::string & osm_ressources::get_api_url(void)const
    {
      return m_current_api;
    }

  //----------------------------------------------------------------------------
  void osm_ressources::set_replication_domain(const std::string & p_replication_domain)
  {
    m_replication_domain = p_replication_domain;
  }
  //----------------------------------------------------------------------------
  const std::string & osm_ressources::get_replication_domain(void)const
    {
      return m_replication_domain;
    }
  //----------------------------------------------------------------------------
  void osm_ressources::get_user_browse_url(std::string & p_result,
                                           const std::string & p_user_name)const
    {
      std::string l_user = p_user_name;
      char * l_escaped_user = quicky_url_reader::url_reader::escape_string(l_user.c_str());
      p_result = m_user_url +"/"+l_escaped_user;
   }

  //----------------------------------------------------------------------------
  void osm_ressources::get_object_browse_url(std::string & p_result,
                                             const std::string & p_type,
                                             const osm_api_data_types::osm_object::t_osm_id & p_id)
    {
      std::stringstream l_id;
      l_id << p_id;
      p_result =  m_browse_domain + "/" + p_type + "/" + l_id.str();
    }
  //----------------------------------------------------------------------------
  void osm_ressources::get_api_object_url(std::string & p_result,
                                          const std::string & p_type,
                                          const osm_api_data_types::osm_object::t_osm_id & p_id,
                                          const osm_api_data_types::osm_core_element::t_osm_version & p_version)
    {
      
      std::stringstream l_id;
      l_id << p_id;
      p_result = m_current_api+"/"+p_type+"/"+l_id.str();
      if(p_version)
	{
	  std::stringstream l_version;
	  l_version << p_version;
	  p_result += "/" + l_version.str();
	}
    }

}
#endif // _OSM_RESSOURCES_H_
//EOF
