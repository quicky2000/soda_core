#ifndef _COMMON_API_WRAPPER_H_
#define _COMMON_API_WRAPPER_H_

#include "common_api_if.h"
#include "osm_ressources.h"

namespace osm_diff_watcher
{
  class common_api_wrapper
  {
  public:
    
    static const char * get_api_version(void);
    inline static void get_user_subscription_date(const std::string & p_name,std::string & p_date);
    static void register_function(void **,uint32_t);
  private:
    static osm_ressources m_ressources;
  };
  
  //----------------------------------------------------------------------------
  void common_api_wrapper::get_user_subscription_date(const std::string & p_name,std::string & p_date)
    {
      m_ressources.get_user_subscription_date(p_name,p_date);
    }
}
#endif // _COMMON_API_WRAPPER_H_
//EOF
