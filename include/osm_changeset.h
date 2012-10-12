#ifndef _OSM_CHANGESET_H_
#define _OSM_CHANGESET_H_

#include "osm_object.h"
#include "osm_bounding_box.h"

namespace osm_diff_watcher
{
  class osm_changeset: public osm_object
  {
  public:
    inline osm_changeset(const t_osm_id  & p_id,
                         const t_osm_id & p_user_id,
                         const std::string & p_user,
                         const std::string & p_created_at,
                         const std::string & p_closed_at,
                         bool p_open,
                         const osm_bounding_box & p_bounding_box
                         );
    inline const std::string & get_created_at(void)const;
    inline const std::string & get_closed_at(void)const;
    inline bool is_open(void)const;
    inline const osm_bounding_box & get_bounding_box(void)const;
  private:
    const std::string m_created_at;
    const std::string m_closed_at;
    bool m_open;
    osm_bounding_box m_bounding_box;
  }
  ;

  //----------------------------------------------------------------------------
  osm_changeset::osm_changeset(const t_osm_id  & p_id,
                               const t_osm_id & p_user_id,
                               const std::string & p_user,
                               const std::string & p_created_at,
                               const std::string & p_closed_at,
                               bool p_open,
                               const osm_bounding_box & p_bounding_box
                               ):
    osm_object(p_id,p_user_id,p_user),
    m_created_at(p_created_at),
    m_closed_at(p_closed_at),
    m_open(p_open),
    m_bounding_box(p_bounding_box)
      {
      }

    //----------------------------------------------------------------------------
    const std::string & osm_changeset::get_created_at(void)const
      {
        return m_created_at;
      }

    //----------------------------------------------------------------------------
    const std::string & osm_changeset::get_closed_at(void)const
      {
        return m_closed_at;
      }

    //----------------------------------------------------------------------------
    bool osm_changeset::is_open(void)const
    {
      return m_open;
    }

    //----------------------------------------------------------------------------
    const osm_bounding_box & osm_changeset::get_bounding_box(void)const
      {
        return m_bounding_box;
      }
  
}
#endif // _OSM_CHANGESET_H_
//EOF
