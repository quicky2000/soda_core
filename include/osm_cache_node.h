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
#ifndef _OSM_CACHE_NODE_H_
#define _OSM_CACHE_NODE_H_

#include "osm_cache_core_element.h"

namespace osm_diff_watcher
{
  class osm_cache_node: public osm_cache_core_element
  {

  public:
    inline osm_cache_node(void);
    inline osm_cache_node(const osm_api_data_types::osm_object::t_osm_id & p_id,
			  const osm_api_data_types::osm_core_element::t_osm_version & p_version,
			  const osm_api_data_types::osm_object::t_osm_id & p_uid,
			  const std::string & p_timestamp,
			  const osm_api_data_types::osm_object::t_osm_id & p_changeset,
			  const float & p_lat,
			  const float & p_lon);
    inline osm_cache_node(const osm_cache_core_element & p_core_element,
                          const float & p_lat,
                          const float & p_lon);
    inline const float & get_lat(void)const;
    inline const float & get_lon(void)const;
    inline const std::string & get_type(void)const;
    inline static const std::string & get_class_type(void);

  private:
    float m_lat;
    float m_lon;
    static const std::string m_class_type;
  };

  //------------------------------------------------------------------------------
  osm_cache_node::osm_cache_node(const osm_api_data_types::osm_object::t_osm_id & p_id,
				 const osm_api_data_types::osm_core_element::t_osm_version & p_version,
				 const osm_api_data_types::osm_object::t_osm_id & p_uid,
				 const std::string & p_timestamp,
				 const osm_api_data_types::osm_object::t_osm_id & p_changeset,
				 const float & p_lat,
				 const float & p_lon):
    osm_cache_core_element(p_id,
			   p_version,
			   p_uid,
			   p_timestamp,
			   p_changeset),
    m_lat(p_lat),
    m_lon(p_lon)
    {
    }

  //------------------------------------------------------------------------------
  osm_cache_node::osm_cache_node(const osm_cache_core_element & p_core_element,
                                 const float & p_lat,
                                 const float & p_lon):
    osm_cache_core_element(p_core_element),
    m_lat(p_lat),
    m_lon(p_lon)
      {
      }
    
    //------------------------------------------------------------------------------
    osm_cache_node::osm_cache_node():
      osm_cache_core_element(),
      m_lat(0.0),
      m_lon(0.0)
        {
        }

      //------------------------------------------------------------------------------
      const float & osm_cache_node::get_lat(void)const
        {
          return m_lat;
        }

      //------------------------------------------------------------------------------
      const float & osm_cache_node::get_lon(void)const
        {
          return m_lon;
        }

      //------------------------------------------------------------------------------
      const std::string & osm_cache_node::get_type(void)const
        {
          return m_class_type;
        }

      //------------------------------------------------------------------------------
      const std::string & osm_cache_node::get_class_type(void)
        {
          return m_class_type;
        }
}
#endif //  _OSM_CACHE_NODE_H_
//EOF
