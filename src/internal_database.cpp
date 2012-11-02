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
#include "internal_database.h"
#include "osm_diff_state.h"
#include <sqlite3.h>
#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdlib>

namespace osm_diff_watcher
{

  //----------------------------------------------------------------------------
  internal_database::internal_database(const std::string & p_name):
    m_db(NULL),
    m_table("internal_information")
  {
    // Opening the database
    int l_status = sqlite3_open(p_name.c_str(), &m_db);
    if(l_status == SQLITE_OK)
      {
	m_table.set_db(m_db);
      }
    else
      {
        std::cerr << "Can't open database \"" << p_name << "\" : " << sqlite3_errmsg(m_db) << std::endl ;
      }
    std::cout << "Database \"" << p_name << "\" successfully opened" << std::endl ; 

  }

  //----------------------------------------------------------------------------
  internal_database::~internal_database(void)
    {
      sqlite3_close(m_db);     
    }

  //----------------------------------------------------------------------------
  void internal_database::store(const osm_diff_analyzer_if::osm_diff_state & p_diff_state)
  {
    // Convert sequence number to string
    std::stringstream l_2string;
    l_2string << p_diff_state.get_sequence_number();
   
    // Store sequence number
    std::pair<std::string,std::string> l_information;
    if(m_table.get("latest_sequence_number",l_information))
      {
	m_table.update("latest_sequence_number",l_2string.str());
      }
    else
      {
	m_table.create("latest_sequence_number",l_2string.str());
      }

    // Store sequence timestamp
    if(m_table.get("latest_sequence_timestamp",l_information))
      {
	m_table.update("latest_sequence_timestamp",p_diff_state.get_timestamp());
      }
    else
      {
	m_table.create("latest_sequence_timestamp",p_diff_state.get_timestamp());
      }
  }

  //----------------------------------------------------------------------------
  const uint64_t internal_database::get_latest_sequence_number(void)
  {
    // Store sequence number
    std::pair<std::string,std::string> l_information;
    return ( m_table.get("latest_sequence_number",l_information)? strtoull(l_information.second.c_str(),NULL,10) : 0);
  }

}
//EOF
