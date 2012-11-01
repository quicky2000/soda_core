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
