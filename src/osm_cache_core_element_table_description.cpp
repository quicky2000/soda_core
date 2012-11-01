#include "osm_cache_core_element_table_description.h"

namespace osm_diff_watcher
{
  const std::string osm_cache_base_table_description<osm_cache_core_element>::m_class_type = "osm_cache_core_element";
  const std::string osm_cache_base_table_description<osm_cache_core_element>::m_table_fields_declaration = "Version INTEGER, Uid INTEGER, Timestamp TEXT, Changeset INTEGER";
  const std::string osm_cache_base_table_description<osm_cache_core_element>::m_table_fields = "Version, Uid, Timestamp, Changeset";
  const std::string osm_cache_base_table_description<osm_cache_core_element>::m_update_fields = "Version = $version ,Uid = $uid, Timestamp = $timestamp, Changeset = $changeset";
  const std::string osm_cache_base_table_description<osm_cache_core_element>::m_field_values = "$version, $uid, $timestamp, $changeset";
}
//EOF
