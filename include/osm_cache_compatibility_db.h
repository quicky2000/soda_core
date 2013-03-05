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
#ifndef _OSM_CACHE_COMPATIBILITY_DB_
#define _OSM_CACHE_COMPATIBILITY_DB_

// Sql table types
#include "osm_cache_named_table.h"
#include "information_table.h"

// Elements to be stored in table
#include "osm_cache_user.h"
// Tables descriptions
#include "osm_cache_user_table_description.h"

class sqlite3;


namespace osm_diff_watcher
{
  class soda_Ui_if;
  class osm_cache_compatibility_db
  {
  public:
    osm_cache_compatibility_db(const std::string & p_name,const std::string & p_current_version,soda_Ui_if & p_ui);
    ~osm_cache_compatibility_db(void);
  private:
    void upgrade(const std::string & p_from_version);
    void upgrade_from_0_1(void);

    sqlite3 * m_db;

    // Element tables
    osm_cache_named_table<osm_cache_user> m_user_table;
    information_table m_informations;
    soda_Ui_if & m_ui;
  };

}
#endif // _OSM_CACHE_COMPATIBILITY_DB_
//EOF
