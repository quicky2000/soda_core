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
    const osm_diff_analyzer_if::osm_diff_state * get_minute_diff_state(void)const;
    static osm_ressources & instance(void);
    static void remove_instance(void);
  private:
    osm_ressources(void);

    const std::string m_domain;
    const std::string m_data_domain;
    const std::string m_redaction_domain;
    const std::string m_replication_domain;
    const std::string m_cc_by_sa_domain;
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

}
#endif // _OSM_RESSOURCES_H_
//EOF
