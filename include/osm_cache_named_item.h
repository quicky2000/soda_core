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
#ifndef _OSM_CACHE_NAMED_ITEM_H_
#define _OSM_CACHE_NAMED_ITEM_H_

#include "osm_cache_item.h"
#include <iostream>
#include <string>
#include <inttypes.h>
#include <cassert>

namespace osm_diff_watcher
{
  class osm_cache_named_item: public osm_cache_item
  {

    friend std::ostream& operator<<(std::ostream& s, const osm_cache_named_item & p_osm_cache_named_item);

  public:
    inline osm_cache_named_item(void);
    inline osm_cache_named_item(const std::string &p_name);
    inline osm_cache_named_item(uint32_t p_id, const std::string &p_name);
    inline const std::string & get_name(void)const;
    inline void set_name(const std::string & p_name);
    inline const std::string & get_type(void)const;
  private:
    std::string m_name;
    static const std::string m_class_type;
  
  };

  inline std::ostream& operator<<(std::ostream& s, const osm_cache_named_item & p_osm_cache_named_item);

  //------------------------------------------------------------------------------
  osm_cache_named_item::osm_cache_named_item(void):
    osm_cache_item(),
    m_name("")
      {
      }

    //------------------------------------------------------------------------------
    osm_cache_named_item::osm_cache_named_item(const std::string &p_name):
      osm_cache_item(),
      m_name(p_name)
	{
	}

      //------------------------------------------------------------------------------
      osm_cache_named_item::osm_cache_named_item(uint32_t p_id, const std::string &p_name):
	osm_cache_item(p_id),
	m_name(p_name)
	  {
	  }

	//------------------------------------------------------------------------------
	const std::string & osm_cache_named_item::get_name(void)const
	  {
	    return m_name;
	  }

	//------------------------------------------------------------------------------
	void osm_cache_named_item::set_name(const std::string & p_name)
	{
	  m_name = p_name;
	}

	//------------------------------------------------------------------------------
	std::ostream& operator<<(std::ostream& s, const osm_cache_named_item & p_osm_cache_named_item)
	  {
	    s << p_osm_cache_named_item.get_type() << "{Id=" << p_osm_cache_named_item.get_id() << ",Name=\"" << p_osm_cache_named_item.m_name << "\"}" ;
	    return s;
	  }

	//------------------------------------------------------------------------------
	const std::string & osm_cache_named_item::get_type(void)const
	  {
	    return m_class_type;
	  }

}

#endif // _OSM_CACHE_NAMED_ITEM_H_
//EOF
