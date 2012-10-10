#ifndef _OSM_WAY_H_
#define _OSM_WAY_H_

#include "osm_object.h"
#include <vector>

namespace osm_diff_watcher
{
  class osm_way:public osm_object
  {
  public:
    inline osm_way(const t_osm_id  & p_id,
                   const std::string & p_timestamp,
                   const t_osm_version & p_version,
                   const t_osm_id & p_changeset,
                   const t_osm_id & p_user_id,
                   const std::string & p_user,
                   bool p_visible = true);
    inline void add_node(const osm_object::t_osm_id & p_node_ref);
    inline void remove_node(const osm_object::t_osm_id & p_node_ref);
    inline bool contains_node(const osm_object::t_osm_id & p_node_ref)const;

    // Utilitie
    static inline const std::string & get_type_str(void);
  private:
    std::vector<osm_object::t_osm_id> m_node_refs;
    static const std::string m_type_str;
  };

    //------------------------------------------------------------------------------
    const std::string & osm_way::get_type_str(void)
      {
        return m_type_str;
      }

  //------------------------------------------------------------------------------
  osm_way::osm_way(const t_osm_id  & p_id,
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
    void osm_way::add_node(const osm_object::t_osm_id & p_node_ref)
    {
      m_node_refs.push_back(p_node_ref);
    }

    //----------------------------------------------------------------------------
    void osm_way::remove_node(const osm_object::t_osm_id & p_node_ref)
    {
      std::vector<osm_object::t_osm_id>::iterator l_iter = m_node_refs.begin();
      std::vector<osm_object::t_osm_id>::iterator l_iter_end = m_node_refs.end();
      bool l_found = false;
      while(l_iter != l_iter_end && !l_found)
        {
          if(*l_iter == p_node_ref)
            {
              m_node_refs.erase(l_iter);
              l_found = true;
            }
          ++l_iter;
        }

    }

    //----------------------------------------------------------------------------
    bool osm_way::contains_node(const osm_object::t_osm_id & p_node_ref)const
    {
      std::vector<osm_object::t_osm_id>::const_iterator l_iter = m_node_refs.begin();
      std::vector<osm_object::t_osm_id>::const_iterator l_iter_end = m_node_refs.end();
      while(l_iter != l_iter_end)
        {
          if(*l_iter == p_node_ref) return true;
          ++ l_iter;
        }
      return false;
    }
}

#endif // _OSM_WAY_H_
// EOF
