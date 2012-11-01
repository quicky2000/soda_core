#ifndef _OSM_CACHE_OBJECT_TAG_H_
#define _OSM_CACHE_OBJECT_TAG_H_

#include "osm_cache_versionned_item.h"
#include "osm_api_data_types.h"

namespace osm_diff_watcher
{
  class osm_cache_object_tag: public osm_cache_versionned_item
  {

  public:
    inline osm_cache_object_tag(void);
    inline osm_cache_object_tag(const osm_cache_object_tag & p_object_tag);
    inline osm_cache_object_tag(const uint64_t & p_id,
                                const osm_api_data_types::osm_core_element::t_osm_version & p_version,
                                const osm_api_data_types::osm_object::t_osm_id p_tag_name_id,
                                const osm_api_data_types::osm_object::t_osm_id p_tag_value_id);

    inline const osm_api_data_types::osm_object::t_osm_id & get_tag_name_id(void)const;
    inline const osm_api_data_types::osm_object::t_osm_id & get_tag_value_id(void)const;

    inline const std::string & get_type(void)const;
    inline static const std::string & get_class_type(void);

  private:
    osm_api_data_types::osm_object::t_osm_id m_tag_name_id;
    osm_api_data_types::osm_object::t_osm_id m_tag_value_id;

    static const std::string m_class_type;
  };

  //------------------------------------------------------------------------------
  osm_cache_object_tag::osm_cache_object_tag():
    osm_cache_versionned_item(),
    m_tag_name_id(0),
    m_tag_value_id(0)
      {
      }
  //------------------------------------------------------------------------------
    osm_cache_object_tag::osm_cache_object_tag(const osm_cache_object_tag & p_object_tag):
      osm_cache_versionned_item(p_object_tag),
      m_tag_name_id(p_object_tag.m_tag_name_id),
      m_tag_value_id(p_object_tag.m_tag_value_id)
      {
      }

      //------------------------------------------------------------------------------
      osm_cache_object_tag::osm_cache_object_tag(const uint64_t & p_id,
                                                 const osm_api_data_types::osm_core_element::t_osm_version & p_version,
                                                 const osm_api_data_types::osm_object::t_osm_id p_tag_name_id,
                                                 const osm_api_data_types::osm_object::t_osm_id p_tag_value_id):
        osm_cache_versionned_item(p_id,p_version),
        m_tag_name_id(p_tag_name_id),
        m_tag_value_id(p_tag_value_id)
          {
          }
      
        //------------------------------------------------------------------------------
        const osm_api_data_types::osm_object::t_osm_id & osm_cache_object_tag::get_tag_name_id(void)const
          {
            return m_tag_name_id;
          }

        //------------------------------------------------------------------------------
        const osm_api_data_types::osm_object::t_osm_id & osm_cache_object_tag::get_tag_value_id(void)const
          {
            return m_tag_value_id;
          }


        //------------------------------------------------------------------------------
        const std::string & osm_cache_object_tag::get_type(void)const
          {
            return m_class_type;
          }

        //------------------------------------------------------------------------------
        const std::string & osm_cache_object_tag::get_class_type(void)
          {
            return m_class_type;
          }
}
#endif //  _OSM_CACHE_OBJECT_TAG_H_
//EOF
