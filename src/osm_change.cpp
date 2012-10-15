#include "osm_change.h"

namespace osm_diff_watcher
{
  std::map<std::string,osm_change::t_osm_change_type> osm_change::m_osm_change_types;
  std::map<osm_change::t_osm_change_type,std::string> osm_change::m_osm_change_types_str;
}

//EOF
