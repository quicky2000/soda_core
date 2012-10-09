#ifndef _OSM_CHANGE_H_
#define _OSM_CHANGE_H_

#include <map>
#include <string>
#include <cassert>

namespace osm_diff_watcher
{
  class osm_change
  {
  public:
    typedef enum {CREATION,MODIFICATION,DELETION,INTERNAL_INVALID}t_osm_change_type;
    virtual const t_osm_change_type get_type(void)const=0;
    inline static t_osm_change_type get_change_type(const std::string & p_name);

    inline virtual ~osm_change(void){}

  private:
    static std::map<std::string,osm_change::t_osm_change_type> m_osm_change_types;
  };

  //------------------------------------------------------------------------------
  osm_change::t_osm_change_type osm_change::get_change_type(const std::string & p_name)
  {
    if(m_osm_change_types.size()==0)
      {
	m_osm_change_types.insert(std::map<std::string,osm_change::t_osm_change_type>::value_type("create",osm_change::CREATION));
	m_osm_change_types.insert(std::map<std::string,osm_change::t_osm_change_type>::value_type("modify",osm_change::MODIFICATION));
	m_osm_change_types.insert(std::map<std::string,osm_change::t_osm_change_type>::value_type("delete",osm_change::DELETION));
      }
    std::map<std::string,osm_change::t_osm_change_type>::const_iterator l_iter = m_osm_change_types.find(p_name);
    assert(l_iter != m_osm_change_types.end());
    return l_iter->second;
  }
  

  template<class T>
    class osm_change_generic:public osm_change
    {
    public:
      inline osm_change_generic(const osm_change::t_osm_change_type & p_change_type,const T * const p_object);
      inline const osm_change::t_osm_change_type get_type(void)const;
      inline ~osm_change_generic(void);
    private:
      const T * const m_object;
      const osm_change::t_osm_change_type m_type;
    };

  //------------------------------------------------------------------------------
  template<class T>
    osm_change_generic<T>::osm_change_generic(const osm_change::t_osm_change_type & p_change_type,const T * const p_object):
    m_object(p_object),
    m_type(p_change_type)
      {
      }

    //------------------------------------------------------------------------------
    template<class T>
      const typename osm_change::t_osm_change_type osm_change_generic<T>::get_type(void)const
      {
	return m_type;
      }

    //------------------------------------------------------------------------------
    template<class T>
    osm_change_generic<T>::~osm_change_generic(void)
      {
	delete m_object;
      }
}
#endif // _OSM_CHANGE_H_
//EOF
