#ifndef _OSM_CACHE_RELATION_MEMBER_H_
#define _OSM_CACHE_RELATION_MEMBER_H_

#include "osm_cache_way_member.h"
#include "osm_api_data_types.h"

namespace osm_diff_watcher
{
  class osm_cache_relation_member: public osm_cache_way_member
  {

  public:
    inline osm_cache_relation_member(void);
    inline osm_cache_relation_member(const osm_cache_way_member & p_way_member,
                                     const osm_api_data_types::osm_core_element::t_osm_type & p_osm_type,
                                     const osm_api_data_types::osm_object::t_osm_id & p_role_id
                                     );
    inline osm_cache_relation_member(const uint64_t & p_id,
                                     const osm_api_data_types::osm_core_element::t_osm_version & p_version,
                                     const uint32_t & p_index,
                                     const osm_api_data_types::osm_object::t_osm_id & p_ref,
                                     const osm_api_data_types::osm_core_element::t_osm_type & p_osm_type,
                                     const osm_api_data_types::osm_object::t_osm_id & p_role_id
                                     );

    inline const osm_api_data_types::osm_core_element::t_osm_type & get_osm_type(void)const;
    inline const osm_api_data_types::osm_object::t_osm_id & get_role_id(void)const;

    inline const std::string & get_type(void)const;
    inline static const std::string & get_class_type(void);

  private:
    osm_api_data_types::osm_core_element::t_osm_type m_osm_type;
    osm_api_data_types::osm_object::t_osm_id m_role_id;

    static const std::string m_class_type;
  };

  //------------------------------------------------------------------------------
  osm_cache_relation_member::osm_cache_relation_member():
    osm_cache_way_member(),
    m_osm_type(osm_api_data_types::osm_core_element::INTERNAL_INVALID),
    m_role_id(0)
      {
      }
  //------------------------------------------------------------------------------
    osm_cache_relation_member::osm_cache_relation_member(const osm_cache_way_member & p_way_member,
                                                         const osm_api_data_types::osm_core_element::t_osm_type & p_osm_type,
                                                         const osm_api_data_types::osm_object::t_osm_id & p_role_id
                                                         ):
      osm_cache_way_member(p_way_member),
      m_osm_type(p_osm_type),
      m_role_id(p_role_id)
      {
      }

    //------------------------------------------------------------------------------
    osm_cache_relation_member::osm_cache_relation_member(const uint64_t & p_id,
                                                         const osm_api_data_types::osm_core_element::t_osm_version & p_version,
                                                         const uint32_t & p_index,
                                                         const osm_api_data_types::osm_object::t_osm_id & p_ref,
                                                         const osm_api_data_types::osm_core_element::t_osm_type & p_osm_type,
                                                         const osm_api_data_types::osm_object::t_osm_id & p_role_id):
      osm_cache_way_member(p_id,p_version,p_index,p_ref),
      m_osm_type(p_osm_type),
      m_role_id(p_role_id)
        {
        }
      
      //------------------------------------------------------------------------------
      const osm_api_data_types::osm_core_element::t_osm_type & osm_cache_relation_member::get_osm_type(void)const
        {
          return m_osm_type;
        }

      //------------------------------------------------------------------------------
      const osm_api_data_types::osm_object::t_osm_id & osm_cache_relation_member::get_role_id(void)const
        {
          return m_role_id;
        }


      //------------------------------------------------------------------------------
      const std::string & osm_cache_relation_member::get_type(void)const
        {
          return m_class_type;
        }

      //------------------------------------------------------------------------------
      const std::string & osm_cache_relation_member::get_class_type(void)
        {
          return m_class_type;
        }
}
#endif //  _OSM_CACHE_RELATION_MEMBER_H_
//EOF
