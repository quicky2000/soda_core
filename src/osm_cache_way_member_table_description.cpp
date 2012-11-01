#include "osm_cache_way_member_table_description.h"

namespace osm_diff_watcher
{
  const std::string osm_cache_base_table_description<osm_cache_way_member>::m_class_type = "way_member";
  const std::string osm_cache_base_table_description<osm_cache_way_member>::m_table_fields_declaration = "Version INTEGER, Position INTEGER, Ref INTEGER";
  const std::string osm_cache_base_table_description<osm_cache_way_member>::m_table_fields = "Version, Position, Ref";
  const std::string osm_cache_base_table_description<osm_cache_way_member>::m_update_fields = "Version = $version ,Position = $position, Ref = $ref";
  const std::string osm_cache_base_table_description<osm_cache_way_member>::m_field_values = "$version, $position, $ref";
}
//EOF
