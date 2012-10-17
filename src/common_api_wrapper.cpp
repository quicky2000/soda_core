#include "common_api_wrapper.h"

#include <cassert>

namespace osm_diff_watcher
{
  //----------------------------------------------------------------------------
  void common_api_wrapper::register_function(void ** p_api_ptr,uint32_t p_size)
  {
    assert(p_size == COMMON_API_IF_SIZE);
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_API_VERSION] = (void*) common_api_wrapper::get_api_version;
    p_api_ptr[osm_diff_analyzer_if::common_api_if::GET_USER_SUBSCRIPTION_DATE] = (void*) common_api_wrapper::get_user_subscription_date;

  }

  //----------------------------------------------------------------------------
  const char *common_api_wrapper::get_api_version(void)
  {
    return COMMON_API_IF_VERSION;
  }

  osm_ressources common_api_wrapper::m_ressources;
}
//EOF
