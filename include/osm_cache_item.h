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
#ifndef _OSM_CACHE_ITEM_H_
#define _OSM_CACHE_ITEM_H_

#include "osm_api_data_types.h"
#include <iostream>
#include <string>
#include <stdint.h>
#include <cassert>

namespace osm_diff_watcher
{
  class osm_cache_item
  {

    friend std::ostream& operator<<(std::ostream& s, const osm_cache_item & p_osm_cache_item);

  public:
    inline osm_cache_item(void);
    inline osm_cache_item(const osm_api_data_types::osm_object::t_osm_id & p_id);
    inline const osm_api_data_types::osm_object::t_osm_id & get_id(void)const;
    inline void set_id(const osm_api_data_types::osm_object::t_osm_id & p_id);
    virtual const std::string & get_type(void)const=0;
    inline virtual ~osm_cache_item(void){}
  private:
    osm_api_data_types::osm_object::t_osm_id m_id;
  
  };

  inline std::ostream& operator<<(std::ostream& s, const osm_cache_item & p_osm_cache_item);

  //------------------------------------------------------------------------------
  osm_cache_item::osm_cache_item():
    m_id(0)
    {
    }

    //------------------------------------------------------------------------------
    osm_cache_item::osm_cache_item(const osm_api_data_types::osm_object::t_osm_id & p_id):
      m_id(p_id)
      {
	assert(m_id);
      }

      //------------------------------------------------------------------------------
      void osm_cache_item::set_id(const osm_api_data_types::osm_object::t_osm_id & p_id)
      {
	assert(!m_id);
	assert(p_id);
	m_id = p_id;
      }

      //------------------------------------------------------------------------------
      const osm_api_data_types::osm_object::t_osm_id & osm_cache_item::get_id(void)const
      {
	return m_id;
      }

      //------------------------------------------------------------------------------
      std::ostream& operator<<(std::ostream& s, const osm_cache_item & p_osm_cache_item)
	{
	  s << p_osm_cache_item.get_type() << "{Id=" << p_osm_cache_item.m_id << "}" ;
	  return s;
	}

}
#endif // _OSM_CACHE_ITEM_H_
//EOF
