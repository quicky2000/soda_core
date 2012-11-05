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
#include "information_table.h"
#include <iostream>
#include <cstdlib>
#include <cassert>

using namespace std;

#if SQLITE_VERSION_NUMBER < 3006000
#define sqlite3_prepare_v2 sqlite3_prepare
#endif


namespace osm_diff_watcher
{
  //------------------------------------------------------------------------------
  information_table::information_table(const std::string & p_name):
    m_table_name(p_name),
    m_db(NULL),
    m_create_stmt(NULL),
    m_update_stmt(NULL),
    m_delete_stmt(NULL),
    m_get_stmt(NULL),
    m_get_all_stmt(NULL)
  {
  }

  //------------------------------------------------------------------------------
  information_table::~information_table(void)
  {
    sqlite3_finalize(m_get_all_stmt);
    sqlite3_finalize(m_get_stmt);
    sqlite3_finalize(m_delete_stmt);  
    sqlite3_finalize(m_update_stmt);  
    sqlite3_finalize(m_create_stmt);  
    std::cout << "Table " << m_table_name << " end of destruction" << std::endl ;
  }

  //------------------------------------------------------------------------------
  sqlite3 * information_table::get_db(void)
  {
    return m_db;
  }

  //------------------------------------------------------------------------------
  void information_table::set_db(sqlite3 *p_db)
  {
    assert(p_db);
    m_db = p_db;

    sqlite3_stmt *l_stmt = NULL;

    // Creation to table
    //--------------------
    int l_status = sqlite3_prepare_v2(m_db,("CREATE TABLE IF NOT EXISTS " + m_table_name + " ( Key TEXT PRIMARY KEY, Value TEXT);").c_str(),-1,&l_stmt,NULL);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during preparation of statement to create " << m_table_name << " table : " << sqlite3_errmsg(m_db) << std::endl ;
	exit(-1);
      }
  
    l_status = sqlite3_step(l_stmt);
    if(!l_status == SQLITE_DONE)
      {
	std::cout << "ERROR during creation of " << m_table_name << " table : " << sqlite3_errmsg(m_db) << std::endl ;
	exit(-1);
      }
    sqlite3_finalize(l_stmt);  

    // Preparing named_item create statement
    //-------------------------------------------
    l_status = sqlite3_prepare_v2(m_db,("INSERT INTO " + m_table_name + " ( Key, Value ) VALUES ( $key , $value );").c_str(),-1,&m_create_stmt,NULL);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during preparation of statement to create " << m_table_name << " item : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }

    // Preparing named_item update statement
    //-------------------------------------------
    l_status = sqlite3_prepare_v2(m_db,("UPDATE " + m_table_name + " SET Value = $value WHERE Key LIKE $key ;").c_str(),-1,&m_update_stmt,NULL);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during preparation of statement to update " << m_table_name << " item : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }

    // Preparing named_item delete statements
    //--------------------------------------------
    l_status = sqlite3_prepare_v2(m_db,("DELETE FROM " + m_table_name + " WHERE Key LIKE $key").c_str(),-1,&m_delete_stmt,NULL);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during preparation of statement to delete " << m_table_name << " item : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }

    // Preparing named_item get statements
    //--------------------------------------------
    l_status = sqlite3_prepare_v2(m_db,("SELECT Key, Value FROM " + m_table_name + " WHERE Key LIKE $key").c_str(),-1,&m_get_stmt,NULL);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during preparation of statement to get " << m_table_name << " item : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }

    // Preparing get_all statements
    //--------------------------------------------
    l_status = sqlite3_prepare_v2(m_db,("SELECT Key, Value FROM " + m_table_name).c_str(),-1,&m_get_all_stmt,NULL);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during preparation of statement to get all " << m_table_name << " items : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }

#ifdef ENABLE_SUCCESS_STATUS_DISPLAY
    std::cout << "Statements OK" << std::endl ;

    std::cout << "Table " << m_table_name << " successfully created" << std::endl ;
#endif  
  }

  //------------------------------------------------------------------------------
  void information_table::create(const std::string & p_key, const std::string & p_value)
  {
    // Binding values to statement
    //----------------------------
    int l_status = sqlite3_bind_text(m_create_stmt,sqlite3_bind_parameter_index(m_create_stmt,"$key"),p_key.c_str(),-1,SQLITE_STATIC);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during binding of key parameter for create statement of " << m_table_name << " : " << sqlite3_errmsg(m_db) << std::endl ;
	exit(-1);
      }  

    l_status = sqlite3_bind_text(m_create_stmt,sqlite3_bind_parameter_index(m_create_stmt,"$value"),p_value.c_str(),-1,SQLITE_STATIC);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during binding of value parameter for create statement of " << m_table_name << " : " << sqlite3_errmsg(m_db) << std::endl ;
	exit(-1);
      }  

    // Executing statement
    //---------------------
    l_status = sqlite3_step(m_create_stmt);
    if( l_status == SQLITE_DONE)
      {
#ifdef ENABLE_SUCCESS_STATUS_DISPLAY
	std::cout << m_table_name << " successfully created" << std::endl ;
#endif
      }
    else
      {
	std::cout << "ERROR during creation of " << m_table_name << " : " << sqlite3_errmsg(m_db) << std::endl ;
	exit(-1);
      }

    // Reset the statement for the next use
    //--------------------------------------
    l_status = sqlite3_reset(m_create_stmt);  
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during reset of " << m_table_name << " create statement : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }

    // Reset bindings because they are now useless
    //--------------------------------------------
#if SQLITE_VERSION_NUMBER > 3006000
    l_status = sqlite3_clear_bindings(m_create_stmt);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during reset of bindings of " << m_table_name << " create statement : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }
#endif

  }

  //------------------------------------------------------------------------------
  void information_table::update(const std::string & p_key, const std::string & p_value)
  {
    // Binding values to statement
    //----------------------------
    int l_status = sqlite3_bind_text(m_update_stmt,sqlite3_bind_parameter_index(m_update_stmt,"$key"),p_key.c_str(),-1,SQLITE_STATIC);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during binding of key parameter for update statement of " << m_table_name << " : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }
  
    l_status = sqlite3_bind_text(m_update_stmt,sqlite3_bind_parameter_index(m_update_stmt,"$value"),p_value.c_str(),-1,SQLITE_STATIC);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during binding of value parameter for create statement of " << m_table_name << " : " << sqlite3_errmsg(m_db) << std::endl ;
	exit(-1);
      }  

    // Executing statement
    //---------------------
    l_status = sqlite3_step(m_update_stmt);
    if( l_status == SQLITE_DONE)
      {
#ifdef ENABLE_SUCCESS_STATUS_DISPLAY
	std::cout << m_table_name << " successfully updated" << std::endl ;
#endif
      }
    else
      {
	std::cout << "ERROR during update of " << m_table_name << " : " << sqlite3_errmsg(m_db) << std::endl ;
	exit(-1);
      }

    // Reset the statement for the next use
    //--------------------------------------
    l_status = sqlite3_reset(m_update_stmt);  
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during reset of " << m_table_name << " update statement : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }

    // Reset bindings because they are now useless
    //--------------------------------------------
#if SQLITE_VERSION_NUMBER > 3006000
    l_status = sqlite3_clear_bindings(m_update_stmt);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during reset of bindings of " << m_table_name << " update statement : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }
#endif


  }

  //------------------------------------------------------------------------------
  void information_table::remove(const std::string & p_key)
  {
    // Binding values to statement
    //----------------------------
    int l_status = sqlite3_bind_text(m_delete_stmt,sqlite3_bind_parameter_index(m_delete_stmt,"$key"),p_key.c_str(),-1,SQLITE_STATIC);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during binding of key parameter for update statement of " << m_table_name << " : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }
  
    // Executing statement
    //---------------------
    l_status = sqlite3_step(m_delete_stmt);
    if( l_status == SQLITE_DONE)
      {
#ifdef ENABLE_SUCCESS_STATUS_DISPLAY
	std::cout << m_table_name << " successfully deleted" << std::endl ;
#endif
      }
    else
      {
	std::cout << "ERROR during delete of " << m_table_name << " : " << sqlite3_errmsg(m_db) << std::endl ;
	exit(-1);
      }

    // Reset the statement for the next use
    //--------------------------------------
    l_status = sqlite3_reset(m_delete_stmt);  
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during reset of " << m_table_name << " delete statement : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }

    // Reset bindings because they are now useless
    //--------------------------------------------
#if SQLITE_VERSION_NUMBER > 3006000
    l_status = sqlite3_clear_bindings(m_delete_stmt);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during reset of bindings of " << m_table_name << " delete statement : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }
#endif
  }

  //------------------------------------------------------------------------------
  uint32_t information_table::get(const std::string & p_key,pair<string,string> & p_data)
  {
  
    uint32_t l_result = 0;

    // Binding values to statement
    //----------------------------
    int l_status = sqlite3_bind_text(m_get_stmt,sqlite3_bind_parameter_index(m_get_stmt,"$key"),p_key.c_str(),-1,SQLITE_STATIC);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during binding of key parameter for update statement of " << m_table_name << " : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }
  
    
    // Executing statement
    //---------------------
    l_status = sqlite3_step(m_get_stmt);
    if( l_status == SQLITE_ROW)
      {
#ifdef ENABLE_SUCCESS_STATUS_DISPLAY
	std::cout << m_table_name << " successfully selected" << std::endl ;
#endif
	p_data = pair<string,string>((const char*)sqlite3_column_text(m_get_stmt,0),(const char*)sqlite3_column_text(m_get_stmt,1));

	// Ensure that ID is unique
	l_status = sqlite3_step(m_get_stmt);
	if( l_status == SQLITE_DONE)
	  {
#ifdef ENABLE_SUCCESS_STATUS_DISPLAY
	    std::cout << m_table_name << " successfully selected done" << std::endl ;
#endif
	    l_result = 1;
	  }
	else
	  {
	    std::cout << "ERROR during selection of " << m_table_name << " : Key \"" << p_key << "\" is not unique " << sqlite3_errmsg(m_db) << std::endl ;
	    exit(-1);
	  }
      }
    else if(l_status != SQLITE_DONE)
      {
	std::cout << "ERROR during selection of " << m_table_name << " : " << sqlite3_errmsg(m_db) << std::endl ;
	exit(-1);
      }



    // Reset the statement for the next use
    //--------------------------------------
    l_status = sqlite3_reset(m_get_stmt);  
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during reset of " << m_table_name << " get statement : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }

    // Reset bindings because they are now useless
    //--------------------------------------------
#if SQLITE_VERSION_NUMBER > 3006000
    l_status = sqlite3_clear_bindings(m_get_stmt);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during reset of bindings of " << m_table_name << " get statement : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }
#endif
    return l_result;
  }

  //------------------------------------------------------------------------------
  void information_table::get_all(std::vector<std::pair<std::string,std::string> > & p_list)
  {

    int l_status = 0;
    // Executing statement
    //---------------------
    while( (l_status = sqlite3_step(m_get_all_stmt)) == SQLITE_ROW)
      {
	p_list.push_back(pair<string,string>((const char*)sqlite3_column_text(m_get_all_stmt,0),(const char*)sqlite3_column_text(m_get_all_stmt,1)));
      }
    if(l_status != SQLITE_DONE)
      {
	std::cout << "ERROR during selection of " << m_table_name << " : " << sqlite3_errmsg(m_db) << std::endl ;
	exit(-1);
      }

#ifdef ENABLE_SUCCESS_STATUS_DISPLAY
    std::cout <<  m_table_name << " successfully listed" << std::endl ;
#endif

    // Reset the statement for the next use
    //--------------------------------------
    l_status = sqlite3_reset(m_get_all_stmt);  
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during reset of " << m_table_name << " get_all statement : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }

  }

}
//EOF
