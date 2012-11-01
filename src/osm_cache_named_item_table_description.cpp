#include "osm_cache_named_item_table_description.h"

namespace osm_diff_watcher
{
  const std::string osm_cache_base_table_description<osm_cache_named_item>::m_class_type = "named_item";
  const std::string osm_cache_base_table_description<osm_cache_named_item>::m_table_fields_declaration = "Name TEXT";
  const std::string osm_cache_base_table_description<osm_cache_named_item>::m_table_fields = "Name";
  const std::string osm_cache_base_table_description<osm_cache_named_item>::m_update_fields = "Name = $name";
  const std::string osm_cache_base_table_description<osm_cache_named_item>::m_field_values = "$name";
}
//EOF
