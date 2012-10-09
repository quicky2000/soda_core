#ifndef _OSM_RELATION_MEMBER_H_
#define _OSM_RELATION_MEMBER_H_

#include "osm_object.h"

namespace osm_diff_watcher
{
  class osm_relation_member
  {
  public:
    inline osm_relation_member(const osm_object::t_osm_type & p_type,
                               const osm_object::t_osm_id & p_object_ref,
                               const std::string & p_role);
    inline const osm_object::t_osm_type & get_type(void)const;
    inline const osm_object::t_osm_id & get_object_ref(void)const;
    inline const std::string & get_role(void)const;
  private:
    const osm_object::t_osm_type m_type;
    const osm_object::t_osm_id m_object_ref;
    const std::string m_role;
  };

  //----------------------------------------------------------------------------
  osm_relation_member::osm_relation_member(const osm_object::t_osm_type & p_type,
                                           const osm_object::t_osm_id & p_object_ref,
                                           const std::string & p_role):
    m_type(p_type),
    m_object_ref(p_object_ref),
    m_role(p_role)
      {
      }
  
    
    //----------------------------------------------------------------------------
    const osm_object::t_osm_type & osm_relation_member::get_type(void)const
      {
        return m_type;
      }

    //----------------------------------------------------------------------------
    const osm_object::t_osm_id & osm_relation_member::get_object_ref(void)const
      {
        return m_object_ref;
      }

    //----------------------------------------------------------------------------
    const std::string & osm_relation_member::get_role(void)const
      {
        return m_role;
      }
}

#endif // _OSM_RELATION_MEMBER_H_
//EOF
