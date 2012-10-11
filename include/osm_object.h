#ifndef _OSM_OBJECT_H_
#define _OSM_OBJECT_H_

#include <string>
#include <map>
#include <cassert>

// TO be replaced by #include <cstdint> to follow C++0x standard
#include <inttypes.h>
#include <iostream>

namespace osm_diff_watcher
{
  class osm_object
  {
  public:
    typedef uint64_t t_osm_id;
    typedef uint32_t t_osm_version;
    // To be replaced by typedef enum class {NODE=0,WAY,RELATION} t_osm_type; to follow C++0x standard
    typedef enum {NODE=0,WAY,RELATION,INTERNAL_INVALID} t_osm_type;

    // Constructor
    inline osm_object(const t_osm_id  & p_id,
		      bool p_visible,
		      const std::string & p_timestamp,
		      const t_osm_version & p_version,
		      const t_osm_id & p_changeset,
		      const t_osm_id & p_user_id,
		      const std::string & p_user);

    // Attributes getter
    inline const t_osm_id & get_id(void)const;
    inline bool is_visible(void)const;
    inline const std::string & get_timestamp(void)const;
    inline const t_osm_version & get_version(void)const;
    inline const t_osm_id & get_changeset(void)const;
    inline const t_osm_id & get_user_id(void)const;
    inline const std::string & get_user(void)const;
    
    // Tags operations
    inline void add_tag(const std::string & p_name, const std::string & p_value);
    inline void remove_tag(const std::string & p_name);
    inline const std::string & get_tag_value(const std::string & p_name)const;
    inline void modify_tag_value(const std::string & p_name,const std::string & p_value);
    inline bool contains_tag(const std::string & p_name)const;
    inline const std::map<std::string,std::string> & get_tags(void)const;

    // Utility
    static inline osm_object::t_osm_type get_osm_type(const std::string & p_name);
    static inline const std::string & get_osm_type_str(const osm_object::t_osm_type & p_type);
    inline void tags_to_string(std::string & p_string)const;
  private:
    t_osm_id m_id;
    bool m_visible;
    std::string m_timestamp;
    t_osm_version m_version;
    t_osm_id m_changeset;
    t_osm_id m_user_id;
    std::string m_user;
    std::map<std::string,std::string> m_tags;

    static std::map<std::string,t_osm_type> m_osm_types;
    static std::map<t_osm_type,std::string> m_osm_types_str;
  };

  //----------------------------------------------------------------------------
  osm_object::t_osm_type osm_object::get_osm_type(const std::string & p_name)
    {
      if(m_osm_types.size()==0)
        {
          m_osm_types.insert(std::map<std::string,t_osm_type>::value_type("node",osm_object::NODE));
          m_osm_types.insert(std::map<std::string,t_osm_type>::value_type("way",osm_object::WAY));
          m_osm_types.insert(std::map<std::string,t_osm_type>::value_type("relation",osm_object::RELATION));
        }
      std::map<std::string,t_osm_type>::const_iterator l_iter = m_osm_types.find(p_name);
      assert(m_osm_types.end() != l_iter);
      return l_iter->second;
    }

  //----------------------------------------------------------------------------
  const std::string & osm_object::get_osm_type_str(const osm_object::t_osm_type & p_type)
    {
      if(m_osm_types_str.size()==0)
        {
          m_osm_types_str.insert(std::map<t_osm_type,std::string>::value_type(osm_object::NODE,"node"));
          m_osm_types_str.insert(std::map<t_osm_type,std::string>::value_type(osm_object::WAY,"way"));
          m_osm_types_str.insert(std::map<t_osm_type,std::string>::value_type(osm_object::RELATION,"relation"));
        }
      std::map<t_osm_type,std::string>::const_iterator l_iter = m_osm_types_str.find(p_type);
      assert(m_osm_types_str.end() != l_iter);
      return l_iter->second;
    }

  // Constructor
  //----------------------------------------------------------------------------
  osm_object::osm_object(const t_osm_id  & p_id,
			 bool p_visible,
			 const std::string & p_timestamp,
			 const t_osm_version & p_version,
			 const osm_object::t_osm_id & p_changeset,
			 const osm_object::t_osm_id & p_user_id,
			 const std::string & p_user):
    m_id(p_id),
    m_visible(p_visible),
    m_timestamp(p_timestamp),
    m_version(p_version),
    m_changeset(p_changeset),
    m_user_id(p_user_id),
    m_user(p_user)
      {
      }

    // Attributes getter
    //----------------------------------------------------------------------------
    const osm_object::t_osm_id & osm_object::get_id(void)const
      {
	return m_id;
      }

    //----------------------------------------------------------------------------
    bool osm_object::is_visible(void)const
    {
      return m_visible;
    }

    //----------------------------------------------------------------------------
    const std::string & osm_object::get_timestamp(void)const
      {
	return m_timestamp;
      }
    //----------------------------------------------------------------------------
    const osm_object::t_osm_version & osm_object::get_version(void)const
      {
	return m_version;
      }
    //----------------------------------------------------------------------------
    const osm_object::t_osm_id & osm_object::get_changeset(void)const
      {
	return m_changeset;
      }

    //----------------------------------------------------------------------------
    const osm_object::t_osm_id & osm_object::get_user_id(void)const
      {
	return m_user_id;
      }
    //----------------------------------------------------------------------------
    const std::string & osm_object::get_user(void)const
      {
	return m_user;
      }
    
    // Tags operations
    //----------------------------------------------------------------------------
    void osm_object::add_tag(const std::string & p_name, const std::string & p_value)
    {
      std::map<std::string,std::string>::const_iterator l_iter = m_tags.find(p_name);
      assert(l_iter ==m_tags.end());
      m_tags.insert(make_pair(p_name,p_value));
    }
    //----------------------------------------------------------------------------
    void osm_object::remove_tag(const std::string & p_name)
    {
      std::map<std::string,std::string>::iterator l_iter = m_tags.find(p_name);
      assert(l_iter != m_tags.end());
      m_tags.erase(l_iter);
    }
    //----------------------------------------------------------------------------
    const std::string & osm_object::get_tag_value(const std::string & p_name)const
      {
	std::map<std::string,std::string>::const_iterator l_iter = m_tags.find(p_name);
	assert(l_iter != m_tags.end());
	return l_iter->second;
      }
    //----------------------------------------------------------------------------
    void osm_object::modify_tag_value(const std::string & p_name,const std::string & p_value)
    {
      std::map<std::string,std::string>::iterator l_iter = m_tags.find(p_name);
      assert(l_iter != m_tags.end());
      l_iter->second = p_value;
    }

    //----------------------------------------------------------------------------
    bool osm_object::contains_tag(const std::string & p_name)const
    {
      std::map<std::string,std::string>::const_iterator l_iter = m_tags.find(p_name);
      return(l_iter != m_tags.end());      
    }

    //----------------------------------------------------------------------------
    const std::map<std::string,std::string> & osm_object::get_tags(void)const
      {
        return m_tags;
      }
    //----------------------------------------------------------------------------
    void osm_object::tags_to_string(std::string & p_string)const
    {
      for(std::map<std::string,std::string>::const_iterator l_iter = m_tags.begin();
            l_iter != m_tags.end();
            ++l_iter)
        {
          p_string += "  " + l_iter->first + "=" + l_iter->second + "\n" ;
        }
    }

}

#endif // _OSM_OBJECT_H_
