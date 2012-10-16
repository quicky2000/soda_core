#include "module_wrapper.h"
#include "module_library_if.h"
#include <cassert>
#include <iostream>

namespace osm_diff_watcher
{
  //----------------------------------------------------------------------------
  module_wrapper::module_wrapper(osm_diff_analyzer_if::module_library_if::t_register_function p_func):
    m_description(NULL),
    m_create(NULL)
  {

    void * l_api_ptr[MODULE_LIBRARY_IF_API_SIZE];
    for(uint32_t l_index = 0 ;l_index < MODULE_LIBRARY_IF_API_SIZE ; ++l_index)
      {
        l_api_ptr[l_index] = NULL;
      }
    p_func(l_api_ptr,MODULE_LIBRARY_IF_API_SIZE);

    osm_diff_analyzer_if::module_library_if::t_get_api_version l_get_api_version = (osm_diff_analyzer_if::module_library_if::t_get_api_version)l_api_ptr[osm_diff_analyzer_if::module_library_if::GET_API_VERSION];
    m_api_version = l_get_api_version();
    osm_diff_analyzer_if::module_library_if::t_get_description l_get_description = (osm_diff_analyzer_if::module_library_if::t_get_description)l_api_ptr[osm_diff_analyzer_if::module_library_if::GET_DESCRIPTION];
    m_description = l_get_description();
    m_create = (osm_diff_analyzer_if::module_library_if::t_create_analyzer)l_api_ptr[osm_diff_analyzer_if::module_library_if::CREATE_ANALYZER];
  }

  //----------------------------------------------------------------------------
  const std::string & module_wrapper::get_api_version(void)const
  {
    return m_api_version;
  }

  //----------------------------------------------------------------------------
  const osm_diff_analyzer_if::analyzer_description_if * module_wrapper::get_description(void)const
  {
    return m_description;
  }

  //----------------------------------------------------------------------------
  osm_diff_analyzer_if::general_analyzer_if* module_wrapper::create_analyzer(const std::string & p_name)
  {
    return m_create(p_name);
  }

}
//EOF
