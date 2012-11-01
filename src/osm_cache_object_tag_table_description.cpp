/*
    This file is part of osm_diff_watcher, Openstreetmap diff analyze framework
    The aim of this software is to provided generic facilities for diff analyzis
    to allow developers to concentrate on analyze rather than diff management 
    infrastructure
    Copyright (C) 2012  Julien Thevenon ( julien_thevenon at yahoo.fr )

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
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
