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
#ifndef _OSM_CACHE_BASE_TABLE_DESCRIPTION_H_
#define _OSM_CACHE_BASE_TABLE_DESCRIPTION_H_

#include <string>

class sqlite3;
class sqlite3_stmt;

namespace osm_diff_watcher
{
  template <class T>
    class osm_cache_base_table_description
    {

    public:
      // Static methods needed for table management in database
      static const std::string & get_class_type(void);
      static const std::string & get_table_fieldsDeclaration(void);
      static const std::string & get_table_fields(void);
      static const std::string & get_update_fields(void);
      static const std::string & get_field_values(void);
      static void bind_item_values(const T & p_type_instance,sqlite3_stmt* p_stmt,sqlite3 *p_db);
      static T get_item_from_row(sqlite3_stmt* p_stmt);
    private:
  
    };
}
#endif // _OSM_CACHE_BASE_TABLE_DESCRIPTION_H_
//EOF
