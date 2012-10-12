#include "osm_core_element.h"

namespace osm_diff_watcher
{
  std::map<std::string,osm_core_element::t_osm_type> osm_core_element::m_osm_types;
  std::map<osm_core_element::t_osm_type,std::string> osm_core_element::m_osm_types_str;
}
//EOF
