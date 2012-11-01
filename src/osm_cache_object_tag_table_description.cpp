#include "osm_cache_object_tag_table_description.h"

namespace osm_diff_watcher
{
  const std::string osm_cache_base_table_description<osm_cache_object_tag>::m_class_type = "osm_cache_object_tag";
  const std::string osm_cache_base_table_description<osm_cache_object_tag>::m_table_fields_declaration = "Version INTEGER, Tag_Name_Id INTEGER, Tag_Value_Id INTEGER";
  const std::string osm_cache_base_table_description<osm_cache_object_tag>::m_table_fields = "Version, Tag_Name_Id, Tag_Value_Id";
  const std::string osm_cache_base_table_description<osm_cache_object_tag>::m_update_fields = "Version = $version ,Tag_Name_Id = $tag_name_id, Tag_Value_Id = $tag_value_id";
  const std::string osm_cache_base_table_description<osm_cache_object_tag>::m_field_values = "$version, $tag_name_id, $tag_value_id";
}
//EOF
