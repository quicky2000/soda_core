#ifndef _OSM_CACHE_WAY_MEMBER_H_
#define _OSM_CACHE_WAY_MEMBER_H_

#include "osm_cache_versionned_item.h"
#include "osm_api_data_types.h"

namespace osm_diff_watcher
{
  class osm_cache_way_member: public osm_cache_versionned_item
  {

  public:
    inline osm_cache_way_member(void);
    inline osm_cache_way_member(const osm_cache_versionned_item & p_item,
                                const uint32_t & p_index,
                                const osm_api_data_types::osm_object::t_osm_id & p_ref);
    inline osm_cache_way_member(const uint64_t & p_id,
                                const osm_api_data_types::osm_core_element::t_osm_version & p_version,
                                const uint32_t & p_index,
                                const osm_api_data_types::osm_object::t_osm_id & p_ref);

    inline const uint32_t & get_index(void)const;
    inline const osm_api_data_types::osm_object::t_osm_id & get_ref(void)const;

    inline const std::string & get_type(void)const;
    inline static const std::string & get_class_type(void);

  private:
    uint32_t m_index;
    osm_api_data_types::osm_object::t_osm_id m_ref;

    static const std::string m_class_type;
  };

  //------------------------------------------------------------------------------
  osm_cache_way_member::osm_cache_way_member():
    osm_cache_versionned_item(),
    m_index(0),
    m_ref(0)
      {
      }
    //------------------------------------------------------------------------------
    osm_cache_way_member::osm_cache_way_member(const osm_cache_versionned_item & p_item,
                                               const uint32_t & p_index,
                                               const osm_api_data_types::osm_object::t_osm_id & p_ref):
      osm_cache_versionned_item(p_item),
      m_index(p_index),
      m_ref(p_ref)
      {
      }

    //------------------------------------------------------------------------------
    osm_cache_way_member::osm_cache_way_member(const uint64_t & p_id,
                                               const osm_api_data_types::osm_core_element::t_osm_version & p_version,
                                               const uint32_t & p_index,
                                               const osm_api_data_types::osm_object::t_osm_id & p_ref):
      osm_cache_versionned_item(p_id,p_version),
      m_index(p_index),
      m_ref(p_ref)
        {
        }
      
      //------------------------------------------------------------------------------
      const uint32_t & osm_cache_way_member::get_index(void)const
        {
          return m_index;
        }

      //------------------------------------------------------------------------------
      const osm_api_data_types::osm_object::t_osm_id & osm_cache_way_member::get_ref(void)const
        {
          return m_ref;
        }


      //------------------------------------------------------------------------------
      const std::string & osm_cache_way_member::get_type(void)const
        {
          return m_class_type;
        }

      //------------------------------------------------------------------------------
      const std::string & osm_cache_way_member::get_class_type(void)
        {
          return m_class_type;
        }
}
#endif //  _OSM_CACHE_WAY_MEMBER_H_
//EOF
