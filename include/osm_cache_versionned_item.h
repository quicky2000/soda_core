#ifndef _OSM_CACHE_VERSIONNED_ITEM_H_
#define _OSM_CACHE_VERSIONNED_ITEM_H_

#include "osm_cache_item.h"
#include <iostream>
#include <string>
#include <stdint.h>
#include <cassert>

namespace osm_diff_watcher
{
  class osm_cache_versionned_item: public osm_cache_item
  {

    friend std::ostream& operator<<(std::ostream& s, const osm_cache_versionned_item & p_osm_cache_versionned_item);

  public:
    inline osm_cache_versionned_item(void);
    inline osm_cache_versionned_item(const osm_cache_versionned_item & p_item);;
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
	assert(m_version);
      }

      //------------------------------------------------------------------------------
      void osm_cache_versionned_item::set_version(const osm_api_data_types::osm_core_element::t_osm_version & p_version)
      {
	assert(!m_version);
	assert(p_version);
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
