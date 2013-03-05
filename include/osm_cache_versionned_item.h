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
#ifndef _OSM_CACHE_VERSIONNED_ITEM_H_
#define _OSM_CACHE_VERSIONNED_ITEM_H_

#include "osm_cache_item.h"
#include "quicky_exception.h"
#include <sstream>
#include <iostream>
#include <string>
#include <inttypes.h>
#include <cassert>

namespace osm_diff_watcher
{
  class osm_cache_versionned_item: public osm_cache_item
  {

    friend std::ostream& operator<<(std::ostream& s, const osm_cache_versionned_item & p_osm_cache_versionned_item);

  public:
    inline osm_cache_versionned_item(void);
    inline osm_cache_versionned_item(const osm_cache_versionned_item & p_item);
    inline osm_cache_versionned_item(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                     const osm_api_data_types::osm_core_element::t_osm_version & p_version);
    inline const osm_api_data_types::osm_core_element::t_osm_version & get_version(void)const;
    inline void set_version(const osm_api_data_types::osm_core_element::t_osm_version & p_version);
  
  private:
    osm_api_data_types::osm_core_element::t_osm_version m_version;
  
  };

  inline std::ostream& operator<<(std::ostream& s, const osm_cache_versionned_item & p_osm_cache_versionned_item);

  //------------------------------------------------------------------------------
  osm_cache_versionned_item::osm_cache_versionned_item():
    osm_cache_item(),
    m_version(0)
    {
    }

  //------------------------------------------------------------------------------
    osm_cache_versionned_item::osm_cache_versionned_item(const osm_cache_versionned_item & p_item):
      osm_cache_item(p_item.get_id()),
      m_version(p_item.m_version)
    {
    }

    //------------------------------------------------------------------------------
    osm_cache_versionned_item::osm_cache_versionned_item(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                         const osm_api_data_types::osm_core_element::t_osm_version & p_version):
      osm_cache_item(p_id),
      m_version(p_version)
      {
        if(!m_version)
          {
            std::stringstream l_stream;
            l_stream << "Cache item for object id " << p_id << " is not allowed to be created with version 0";
            throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
          }
      }

      //------------------------------------------------------------------------------
      void osm_cache_versionned_item::set_version(const osm_api_data_types::osm_core_element::t_osm_version & p_version)
      {
	if(!p_version) throw quicky_exception::quicky_logic_exception("Setting version to 0 is not allowed",__LINE__,__FILE__);
	m_version = p_version;
      }

      //------------------------------------------------------------------------------
      const osm_api_data_types::osm_core_element::t_osm_version & osm_cache_versionned_item::get_version(void)const
      {
	return m_version;
      }

}
#endif // _OSM_CACHE_VERSIONNED_ITEM_H_
//EOF
