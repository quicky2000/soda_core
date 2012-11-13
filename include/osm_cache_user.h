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
#ifndef _OSM_CACHE_USER_H_
#define _OSM_CACHE_USER_H_

#include "osm_cache_named_item.h"

namespace osm_diff_watcher
{
  class osm_cache_user: public osm_cache_named_item
  {

  public:
    inline osm_cache_user(void);
    inline osm_cache_user(const std::string &p_name, const std::string & p_date,const osm_api_data_types::osm_object::t_osm_id & p_latest_changeset);
    inline osm_cache_user(const osm_api_data_types::osm_object::t_osm_id & p_id, const std::string &p_name,const std::string & p_date,const osm_api_data_types::osm_object::t_osm_id & p_latest_changeset);
    inline const std::string & get_type(void)const;
    inline const std::string & get_date(void)const;
    inline const osm_api_data_types::osm_object::t_osm_id & get_latest_changeset(void)const;
    inline static const std::string & get_class_type(void);
  private:
    std::string m_date;
    osm_api_data_types::osm_object::t_osm_id m_latest_changeset;
    static const std::string m_class_type;
  };

  //------------------------------------------------------------------------------
  osm_cache_user::osm_cache_user(void):
    osm_cache_named_item(),
    m_date(""),
    m_latest_changeset(0)
      {
      }
    
    //------------------------------------------------------------------------------
    osm_cache_user::osm_cache_user(const std::string & p_name,
				   const std::string & p_date,
				   const osm_api_data_types::osm_object::t_osm_id & p_latest_changeset):
      osm_cache_named_item(p_name),
      m_date(p_date),
      m_latest_changeset(p_latest_changeset)
	{
	}

      //------------------------------------------------------------------------------
      osm_cache_user::osm_cache_user(const osm_api_data_types::osm_object::t_osm_id & p_id,
				     const std::string &p_name,
				     const std::string & p_date,
				     const osm_api_data_types::osm_object::t_osm_id & p_latest_changeset):
	osm_cache_named_item(p_id,p_name),
	m_date(p_date),
	m_latest_changeset(p_latest_changeset)
	  {
	  }

	//------------------------------------------------------------------------------
	const std::string & osm_cache_user::get_type(void)const
	  {
	    return m_class_type;
	  }

	//------------------------------------------------------------------------------
	const std::string & osm_cache_user::get_date(void)const
	  {
	    return m_date;
	  }

	const osm_api_data_types::osm_object::t_osm_id & osm_cache_user::get_latest_changeset(void)const
	  {
	    return m_latest_changeset;
	  }

	//------------------------------------------------------------------------------
	const std::string & osm_cache_user::get_class_type(void)
	  {
	    return m_class_type;
	  }
}
#endif //  _OSM_CACHE_USER_H_
//EOF
