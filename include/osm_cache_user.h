#ifndef _OSM_CACHE_USER_H_
#define _OSM_CACHE_USER_H_

#include "osm_cache_named_item.h"

namespace osm_diff_watcher
{
  class osm_cache_user: public osm_cache_named_item
  {

  public:
    inline osm_cache_user(void);
    inline osm_cache_user(const std::string &p_name, const std::string & p_date);
    inline osm_cache_user(uint32_t p_id, const std::string &p_name,const std::string & p_date);
    inline const std::string & get_type(void)const;
    inline const std::string & get_date(void)const;
    inline static const std::string & get_class_type(void);

  private:
    std::string m_date;
    static const std::string m_class_type;
  };

  //------------------------------------------------------------------------------
  osm_cache_user::osm_cache_user():
    osm_cache_named_item(),
    m_date("")
    {
    }
    
    //------------------------------------------------------------------------------
    osm_cache_user::osm_cache_user(const std::string &p_name, const std::string & p_date):
      osm_cache_named_item(p_name),
      m_date(p_date)
      {
      }

      //------------------------------------------------------------------------------
      osm_cache_user::osm_cache_user(uint32_t p_id, const std::string &p_name, const std::string & p_date):
	osm_cache_named_item(p_id,p_name),
	m_date(p_date)
	{
	}

	//------------------------------------------------------------------------------
	const std::string & osm_cache_user::get_type(void)const
	  {
	    return m_class_type;
	  }

	//------------------------------------------------------------------------------
	const std::string & osm_cache_user::get_date(void)const
	  {
	    return m_date;
	  }

	//------------------------------------------------------------------------------
	const std::string & osm_cache_user::get_class_type(void)
	  {
	    return m_class_type;
	  }
}
#endif //  _OSM_CACHE_USER_H_
//EOF
