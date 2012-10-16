#ifndef _MODULE_WRAPPER_H_
#define _MODULE_WRAPPER_H_

#include "module_library_if.h"
#include <stdint.h>

namespace osm_diff_watcher
{
  class module_wrapper
  {
  public:
    module_wrapper(osm_diff_analyzer_if::module_library_if::t_register_function p_func);
    const std::string & get_api_version(void)const;
    const osm_diff_analyzer_if::analyzer_description_if * get_description(void)const;
    osm_diff_analyzer_if::general_analyzer_if* create_analyzer(const std::string & p_name);
  private:
    std::string m_api_version;
    osm_diff_analyzer_if::analyzer_description_if * m_description;
    osm_diff_analyzer_if::module_library_if::t_create_analyzer m_create;
  };
}
#endif // _MODULE_WRAPPER_H_
//EOF
