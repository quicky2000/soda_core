#include "osm_cache_user_table_description.h"

namespace osm_diff_watcher
{
  const std::string osm_cache_base_table_description<osm_cache_user>::m_class_type = "user";
  const std::string osm_cache_base_table_description<osm_cache_user>::m_table_fields_declaration = "Name TEXT, Date TEXT";
  const std::string osm_cache_base_table_description<osm_cache_user>::m_table_fields = "Name, Date";
  const std::string osm_cache_base_table_description<osm_cache_user>::m_update_fields = "Name = $name ,Date = $date";
  const std::string osm_cache_base_table_description<osm_cache_user>::m_field_values = "$name, $date";
}
//EOF
