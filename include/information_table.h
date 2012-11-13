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
#ifndef _INFORMATION_TABLE_H_
#define _INFORMATION_TABLE_H_

#include "my_sqlite3.h"
#include <vector>
#include <string>
#include <inttypes.h>

namespace osm_diff_watcher
{
  class information_table
  {
  public:
    void create(const std::string & p_key, const std::string & p_value);
    void update(const std::string & p_key, const std::string & p_value);
    void remove(const std::string & p_key);
    uint32_t get(const std::string & p_key, std::pair<std::string,std::string> & p_data);
    void get_all(std::vector<std::pair<std::string,std::string> > & p_list);

    information_table(const std::string & p_name);
    ~information_table(void);
    void set_db(sqlite3 *p_db);
    sqlite3 * get_db(void);

  private:
    const std::string m_table_name;
    sqlite3 *m_db;
    sqlite3_stmt *m_create_stmt;
    sqlite3_stmt *m_update_stmt;
    sqlite3_stmt *m_delete_stmt;
    sqlite3_stmt *m_get_stmt;
    sqlite3_stmt *m_get_all_stmt;
  };
}
#endif // _INFORMATION_TABLE_H_
//EOF
