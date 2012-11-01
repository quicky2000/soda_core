#ifndef _OSM_CACHE_CORE_ELEMENT_H_
#define _OSM_CACHE_CORE_ELEMENT_H_

#include "osm_cache_versionned_item.h"
#include "osm_api_data_types.h"

namespace osm_diff_watcher
{
  class osm_cache_core_element: public osm_cache_versionned_item
  {

  public:
    inline osm_cache_core_element(void);
    inline osm_cache_core_element(const osm_cache_core_element & p_element);
    inline osm_cache_core_element(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                  const osm_api_data_types::osm_core_element::t_osm_version & p_version,
                                  const osm_api_data_types::osm_object::t_osm_id & p_uid,
                                  const std::string & p_timestamp,
                                  const osm_api_data_types::osm_object::t_osm_id & p_changeset);

    inline const osm_api_data_types::osm_object::t_osm_id & get_uid(void)const;
    inline const std::string & get_timestamp(void)const;
    inline const osm_api_data_types::osm_object::t_osm_id & get_changeset(void)const;

    inline const std::string & get_type(void)const;
    inline static const std::string & get_class_type(void);

  private:
    osm_api_data_types::osm_object::t_osm_id m_uid;
    std::string m_timestamp;
    osm_api_data_types::osm_object::t_osm_id m_changeset;

    static const std::string m_class_type;
  };

  //------------------------------------------------------------------------------
  osm_cache_core_element::osm_cache_core_element(const osm_cache_core_element & p_element):
    osm_cache_versionned_item(p_element),
    m_uid(p_element.m_uid),
    m_timestamp(p_element.m_timestamp),
    m_changeset(p_element.m_changeset)
    {
    }


  //------------------------------------------------------------------------------
  osm_cache_core_element::osm_cache_core_element():
    osm_cache_versionned_item(),
    m_uid(0),
    m_timestamp(""),
    m_changeset(0)
    {
    }
    
      //------------------------------------------------------------------------------
      osm_cache_core_element::osm_cache_core_element(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                     const osm_api_data_types::osm_core_element::t_osm_version & p_version,
                                                     const osm_api_data_types::osm_object::t_osm_id & p_uid,
                                                     const std::string & p_timestamp,
                                                     const osm_api_data_types::osm_object::t_osm_id & p_changeset):
	osm_cache_versionned_item(p_id,p_version),
        m_uid(p_uid),
        m_timestamp(p_timestamp),
        m_changeset(p_changeset)        
	{
	}

	//------------------------------------------------------------------------------
        const osm_api_data_types::osm_object::t_osm_id & osm_cache_core_element::get_uid(void)const
          {
            return m_uid;
          }

	//------------------------------------------------------------------------------
        const std::string & osm_cache_core_element::get_timestamp(void)const
          {
            return m_timestamp;
          }

	//------------------------------------------------------------------------------
        const osm_api_data_types::osm_object::t_osm_id & osm_cache_core_element::get_changeset(void)const
          {
            return m_changeset;
          }

	//------------------------------------------------------------------------------
	const std::string & osm_cache_core_element::get_type(void)const
	  {
	    return m_class_type;
	  }

	//------------------------------------------------------------------------------
	const std::string & osm_cache_core_element::get_class_type(void)
	  {
	    return m_class_type;
	  }
}
#endif //  _OSM_CACHE_CORE_ELEMENT_H_
//EOF
