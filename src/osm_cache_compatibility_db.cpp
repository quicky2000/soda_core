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
#include "osm_cache_compatibility_db.h"
#include <cassert>
#include <iostream>
#include <fstream>

namespace osm_diff_watcher
{
  //----------------------------------------------------------------------------
  osm_cache_compatibility_db::osm_cache_compatibility_db(const std::string & p_name,const std::string & p_current_version):
    m_db(NULL),
    m_informations("information_table")
  {

    std::ifstream l_check_file(p_name.c_str());
    if(l_check_file != NULL)
      {
        l_check_file.close();
        // Opening the database
        int l_status = sqlite3_open_v2(p_name.c_str(), &m_db,SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,NULL);
        if(l_status == SQLITE_OK)
          {
            m_informations.set_db(m_db);
          }
        else
          {
            std::cerr << "Can't open database \"" << p_name << "\" : " << sqlite3_errmsg(m_db) << std::endl ;
          }
        bool l_updated = false;
        do
          {
            std::pair<std::string,std::string> l_info;
            bool l_success = m_informations.get("schema_version",l_info);
            if(l_success)
              {
                if(l_info.second != p_current_version)
                  {
                    upgrade(l_info.second);
                  }
                else
                  {
                    l_updated = true;
                  }
              }
            else
              {
                m_informations.create("schema_version","0.1");
              }
          } while(!l_updated);
      }

  }

  //------------------------------------------------------------------------------
  void osm_cache_compatibility_db::upgrade(const std::string & p_from_version)
  {
    std::cout << "Upgrading from version \"" << p_from_version << "\"" << std::endl ;
    if(p_from_version == "0.1" || p_from_version == "")
      {
	upgrade_from_0_1();
      }
  }

  //------------------------------------------------------------------------------
  void osm_cache_compatibility_db::upgrade_from_0_1(void)
  {
    sqlite3_stmt * l_upgrade_stmt = NULL;
  
    //--------------------------------------------
    // Adding new column to facture table
    //--------------------------------------------
    int l_status = sqlite3_prepare_v2(m_db,
				      ("ALTER TABLE "+m_user_table.get_name()+" ADD COLUMN Latest_changeset INTEGER DEFAULT 0").c_str(),
				      -1,
				      &l_upgrade_stmt,
				      NULL);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during preparation of upgrade from 0.1 statement for " << m_user_table.get_name() << " : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(EXIT_FAILURE);
      }

    l_status = sqlite3_step(l_upgrade_stmt);
    if(!l_status == SQLITE_DONE)
      {
	std::cout << "ERROR during upgrade from 0.1" << std::endl ;
	exit(-1);
      }
    sqlite3_finalize(l_upgrade_stmt);  

    //-------------------------
    // Updating version number
    //-------------------------
    m_informations.update("schema_version","0.2");
  }

  //------------------------------------------------------------------------------
  osm_cache_compatibility_db::~osm_cache_compatibility_db(void)
  {
    sqlite3_close_v2(m_db);
  }

}
//EOF
