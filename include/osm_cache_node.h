#ifndef _OSM_CACHE_NODE_H_
#define _OSM_CACHE_NODE_H_

#include "osm_cache_core_element.h"

namespace osm_diff_watcher
{
  class osm_cache_node: public osm_cache_core_element
  {

  public:
    inline osm_cache_node(void);
    inline osm_cache_node(const osm_api_data_types::osm_object::t_osm_id & p_id,
			  const osm_api_data_types::osm_core_element::t_osm_version & p_version,
			  const osm_api_data_types::osm_object::t_osm_id & p_uid,
			  const std::string & p_timestamp,
			  const osm_api_data_types::osm_object::t_osm_id & p_changeset,
			  const float & p_lat,
			  const float & p_lon);
    inline osm_cache_node(const osm_cache_core_element & p_core_element,
                          const float & p_lat,
                          const float & p_lon);
    inline const float & get_lat(void)const;
    inline const float & get_lon(void)const;
    inline const std::string & get_type(void)const;
    inline static const std::string & get_class_type(void);

  private:
    float m_lat;
    float m_lon;
    static const std::string m_class_type;
  };

  //------------------------------------------------------------------------------
  osm_cache_node::osm_cache_node(const osm_api_data_types::osm_object::t_osm_id & p_id,
				 const osm_api_data_types::osm_core_element::t_osm_version & p_version,
				 const osm_api_data_types::osm_object::t_osm_id & p_uid,
				 const std::string & p_timestamp,
				 const osm_api_data_types::osm_object::t_osm_id & p_changeset,
				 const float & p_lat,
				 const float & p_lon):
    osm_cache_core_element(p_id,
			   p_version,
			   p_uid,
			   p_timestamp,
			   p_changeset),
    m_lat(p_lat),
    m_lon(p_lon)
    {
    }

  //------------------------------------------------------------------------------
  osm_cache_node::osm_cache_node(const osm_cache_core_element & p_core_element,
                                 const float & p_lat,
                                 const float & p_lon):
    osm_cache_core_element(p_core_element),
    m_lat(p_lat),
    m_lon(p_lon)
      {
      }
    
    //------------------------------------------------------------------------------
    osm_cache_node::osm_cache_node():
      osm_cache_core_element(),
      m_lat(0.0),
      m_lon(0.0)
        {
        }

      //------------------------------------------------------------------------------
      const float & osm_cache_node::get_lat(void)const
        {
          return m_lat;
        }

      //------------------------------------------------------------------------------
      const float & osm_cache_node::get_lon(void)const
        {
          return m_lon;
        }

      //------------------------------------------------------------------------------
      const std::string & osm_cache_node::get_type(void)const
        {
          return m_class_type;
        }

      //------------------------------------------------------------------------------
      const std::string & osm_cache_node::get_class_type(void)
        {
          return m_class_type;
        }
}
#endif //  _OSM_CACHE_NODE_H_
//EOF
