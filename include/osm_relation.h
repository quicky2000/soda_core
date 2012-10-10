#ifndef _OSM_RELATION_H_
#define _OSM_RELATION_H_

#include "osm_object.h"
#include "osm_relation_member.h"
#include <vector>

namespace osm_diff_watcher
{
  class osm_relation: public osm_object
  {
  public:
    inline osm_relation(const t_osm_id  & p_id,
                        const std::string & p_timestamp,
                        const t_osm_version & p_version,
                        const t_osm_id & p_changeset,
                        const t_osm_id & p_user_id,
                        const std::string & p_user,
                        bool p_visible = true);

    inline void add_member(const osm_object::t_osm_type & p_type,
                           const osm_object::t_osm_id & p_object_ref,
                           const std::string & p_role);
    inline ~osm_relation(void);

    // Utilitie
    static inline const std::string & get_type_str(void);
  private:
    std::vector<osm_relation_member*> m_members;
    static const std::string m_type_str;
 };

  //------------------------------------------------------------------------------
  const std::string & osm_relation::get_type_str(void)
    {
      return m_type_str;
    }

  //----------------------------------------------------------------------------
  osm_relation::osm_relation(const t_osm_id  & p_id,
                             const std::string & p_timestamp,
                             const t_osm_version & p_version,
                             const t_osm_id & p_changeset,
                             const t_osm_id & p_user_id,
                             const std::string & p_user,
                             bool p_visible):
    osm_object(p_id,true,p_timestamp,p_version,p_changeset,p_user_id,p_user)
    {
    }

  //----------------------------------------------------------------------------
    void osm_relation::add_member(const osm_object::t_osm_type & p_type,
                                  const osm_object::t_osm_id & p_object_ref,
                                  const std::string & p_role)
    {
      m_members.push_back(new osm_relation_member(p_type,p_object_ref,p_role));
    }

    //----------------------------------------------------------------------------
    osm_relation::~osm_relation(void)
    {
      for(std::vector<osm_relation_member*>::iterator l_iter = m_members.begin();
          l_iter != m_members.end();
          ++l_iter)
        {
          delete *l_iter;
        }
    }
   
}
#endif // _OSM_RELATION_H_
//EOF
