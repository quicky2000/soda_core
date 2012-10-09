#ifndef _OSM_NODE_H_
#define _OSM_NODE_H_

#include "osm_object.h"

namespace osm_diff_watcher
{
  class osm_node: public osm_object
  {
  public:
    // Constructor
    inline osm_node(const t_osm_id  & p_id,
		    const float & p_lat,
		    const float & p_lon,
		    const std::string & p_timestamp,
		    const t_osm_version & p_version,
		    const t_osm_id & p_changeset,
		    const t_osm_id & p_user_id,
		    const std::string & p_user,
		    bool p_visible = true);

    // Attributes accessors
    inline const float & get_lat(void)const;
    inline const float & get_lon(void)const;
  private:
    float m_lat;
    float m_lon;
  };

  //------------------------------------------------------------------------------
  osm_node::osm_node(const t_osm_id  & p_id,
		     const float & p_lat,
		     const float & p_lon,
		     const std::string & p_timestamp,
		     const t_osm_version & p_version,
		     const t_osm_id & p_changeset,
		     const t_osm_id & p_user_id,
		     const std::string & p_user,
		     bool p_visible):
    osm_object(p_id,true,p_timestamp,p_version,p_changeset,p_user_id,p_user),
    m_lat(p_lat),
    m_lon(p_lon)
      {
      }

    //------------------------------------------------------------------------------
    const float & osm_node::get_lat(void)const
      {
	return m_lat;
      }

    //------------------------------------------------------------------------------
    const float & osm_node::get_lon(void)const
      {
	return m_lon;
      }
}
#endif // _OSM_NODE_H_
