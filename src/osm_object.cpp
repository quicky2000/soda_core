#include "osm_object.h"

namespace osm_diff_watcher
{
  std::map<std::string,osm_object::t_osm_type> osm_object::m_osm_types;
  std::map<osm_object::t_osm_type,std::string> osm_object::m_osm_types_str;
}

//EOF
