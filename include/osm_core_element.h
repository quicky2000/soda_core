#ifndef _OSM_CORE_ELEMENT_H_
#define _OSM_CORE_ELEMENT_H_

#include "osm_object.h"

namespace osm_diff_watcher
{
  class osm_core_element: public osm_object
  {
  public:
    typedef uint32_t t_osm_version;
    // To be replaced by typedef enum class {NODE=0,WAY,RELATION} t_osm_type; to follow C++0x standard
    typedef enum {NODE=0,WAY,RELATION,INTERNAL_INVALID} t_osm_type;

    // Constructor
    inline osm_core_element(const t_osm_id  & p_id,
                            bool p_visible,
                            const std::string & p_timestamp,
                            const t_osm_version & p_version,
                            const t_osm_id & p_changeset,
                            const t_osm_id & p_user_id,
                            const std::string & p_user);

    // Attributes getter
    inline bool is_visible(void)const;
    inline const std::string & get_timestamp(void)const;
    inline const t_osm_version & get_version(void)const;
    inline const t_osm_id & get_changeset(void)const;

    // Utility
    static inline osm_core_element::t_osm_type get_osm_type(const std::string & p_name);
    static inline const std::string & get_osm_type_str(const osm_core_element::t_osm_type & p_type);
  private:
    bool m_visible;
    std::string m_timestamp;
    t_osm_version m_version;
    t_osm_id m_changeset;

    static std::map<std::string,t_osm_type> m_osm_types;
    static std::map<t_osm_type,std::string> m_osm_types_str;
  };

  //----------------------------------------------------------------------------
  osm_core_element::t_osm_type osm_core_element::get_osm_type(const std::string & p_name)
    {
      if(m_osm_types.size()==0)
        {
          m_osm_types.insert(std::map<std::string,t_osm_type>::value_type("node",osm_core_element::NODE));
          m_osm_types.insert(std::map<std::string,t_osm_type>::value_type("way",osm_core_element::WAY));
          m_osm_types.insert(std::map<std::string,t_osm_type>::value_type("relation",osm_core_element::RELATION));
        }
      std::map<std::string,t_osm_type>::const_iterator l_iter = m_osm_types.find(p_name);
      assert(m_osm_types.end() != l_iter);
      return l_iter->second;
    }

  //----------------------------------------------------------------------------
  const std::string & osm_core_element::get_osm_type_str(const osm_core_element::t_osm_type & p_type)
    {
      if(m_osm_types_str.size()==0)
        {
          m_osm_types_str.insert(std::map<t_osm_type,std::string>::value_type(osm_core_element::NODE,"node"));
          m_osm_types_str.insert(std::map<t_osm_type,std::string>::value_type(osm_core_element::WAY,"way"));
          m_osm_types_str.insert(std::map<t_osm_type,std::string>::value_type(osm_core_element::RELATION,"relation"));
        }
      std::map<t_osm_type,std::string>::const_iterator l_iter = m_osm_types_str.find(p_type);
      assert(m_osm_types_str.end() != l_iter);
      return l_iter->second;
    }

  // Constructor
  //----------------------------------------------------------------------------
  osm_core_element::osm_core_element(const t_osm_id  & p_id,
			 bool p_visible,
			 const std::string & p_timestamp,
			 const t_osm_version & p_version,
			 const osm_core_element::t_osm_id & p_changeset,
			 const osm_core_element::t_osm_id & p_user_id,
			 const std::string & p_user):
    osm_object(p_id,p_user_id,p_user),
    m_visible(p_visible),
    m_timestamp(p_timestamp),
    m_version(p_version),
    m_changeset(p_changeset)
      {
      }

    //----------------------------------------------------------------------------
    bool osm_core_element::is_visible(void)const
    {
      return m_visible;
    }

    //----------------------------------------------------------------------------
    const std::string & osm_core_element::get_timestamp(void)const
      {
	return m_timestamp;
      }
    //----------------------------------------------------------------------------
    const osm_core_element::t_osm_version & osm_core_element::get_version(void)const
      {
	return m_version;
      }
    //----------------------------------------------------------------------------
    const osm_core_element::t_osm_id & osm_core_element::get_changeset(void)const
      {
	return m_changeset;
      }



}

#endif // _OSM_CORE_ELEMENT_H_
//EOF
