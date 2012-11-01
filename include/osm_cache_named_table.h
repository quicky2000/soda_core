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
#ifndef OSM_CACHE_NAMED_TABLE_H
#define OSM_CACHE_NAMED_TABLE_H

#include "osm_cache_base_table.h"

class named_item;

namespace osm_diff_watcher
{
  template <class T> class osm_cache_named_table:public osm_cache_base_table<T>
  {
  public:
    osm_cache_named_table(const std::string & p_name=osm_cache_base_table_description<T>::get_class_type());
    ~osm_cache_named_table(void);
    void set_db(sqlite3 *p_db);

    uint32_t get_by_name(const std::string & p_name, T & p_data);
    void get_by_name(const std::string & p_name,std::vector<T> & p_list, bool p_exact=false);
    void get_all(std::vector<T> & p_list);

  private:
    sqlite3_stmt *m_get_by_name_stmt;
    sqlite3_stmt *m_get_all_ordered_by_name_stmt;
  };

  //------------------------------------------------------------------------------
  template <class T> osm_cache_named_table<T>::osm_cache_named_table(const std::string & p_name):
    osm_cache_base_table<T>(p_name),
    m_get_by_name_stmt(NULL),
    m_get_all_ordered_by_name_stmt(NULL)
      {
      }

    //------------------------------------------------------------------------------
    template <class T> osm_cache_named_table<T>::~osm_cache_named_table(void)
      {
	sqlite3_finalize(m_get_all_ordered_by_name_stmt);
	sqlite3_finalize(m_get_by_name_stmt);
	std::cout << "Table " << this->get_name() << " end of destruction" << std::endl ;
      }

    //------------------------------------------------------------------------------
    template <class T> void osm_cache_named_table<T>::set_db(sqlite3 *p_db)
      {
	osm_cache_base_table<T>::set_db(p_db);
  
	// Preparing get_by_name statements
	//--------------------------------------------
	int l_status = sqlite3_prepare_v2(osm_cache_base_table<T>::get_db(),("SELECT Id,"+osm_cache_base_table_description<T>::get_table_fields()+" FROM " + this->get_name() + " WHERE Name LIKE @name").c_str(),-1,&m_get_by_name_stmt,NULL);
	if(l_status != SQLITE_OK)
	  {
	    std::cout << "ERROR during preparation of statement to get "+osm_cache_base_table_description<T>::get_table_fields()+" item by name : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;     
	    exit(-1);
	  }

	// Preparing get_all statements
	//--------------------------------------------
	l_status = sqlite3_prepare_v2(osm_cache_base_table<T>::get_db(),(std::string("SELECT ") + 
                                                                         "Id," + 
                                                                         osm_cache_base_table_description<T>::get_table_fields() + 
                                                                         " FROM " + 
                                                                         this->get_name() + 
                                                                         " ORDER BY Name"
                                                                         ).c_str(),
				      -1,
				      &m_get_all_ordered_by_name_stmt,
				      NULL);
	if(l_status != SQLITE_OK)
	  {
	    std::cout << "ERROR during preparation of statement to get_all_order_by_name " << this->get_name() << " items : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;     
	    exit(-1);
	  }



      }

    //------------------------------------------------------------------------------
    template <class T> 
      uint32_t osm_cache_named_table<T>::get_by_name(const std::string & p_name, T & p_data)
      {
	uint32_t l_result = 0;
	// Binding values to statement
	//----------------------------
	int l_status = sqlite3_bind_text(m_get_by_name_stmt,sqlite3_bind_parameter_index(m_get_by_name_stmt,"@name"),p_name.c_str(),-1,SQLITE_STATIC);
	if(l_status != SQLITE_OK)
	  {
	    std::cout << "ERROR during binding of name parameter for get_by_name statement of " << this->get_name() << " : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;     
	    exit(-1);
	  }
    
	// Executing statement
	//---------------------
	l_status = sqlite3_step(m_get_by_name_stmt);
	if( l_status == SQLITE_ROW)
	  {
#ifdef ENABLE_SUCCESS_STATUS_DISPLAY
	    std::cout << this->get_name() << " successfully selected" << std::endl ;
#endif
	    p_data = osm_cache_base_table_description<T>::get_item_from_row(m_get_by_name_stmt);

	    // Ensure that ID is unique
	    l_status = sqlite3_step(m_get_by_name_stmt);
	    if( l_status == SQLITE_DONE)
	      {
#ifdef ENABLE_SUCCESS_STATUS_DISPLAY
		std::cout << this->get_name() << " successfully selected done" << std::endl ;
#endif
		l_result = 1;
	      }
	    else
	      {
		std::cout << "ERROR during selection of " << this->get_name() << " : Name \"" << p_name << "\" is not unique " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;
		exit(-1);
	      }
	  }
	else if(l_status != SQLITE_DONE)
	  {
	    std::cout << "ERROR during selection of " << this->get_name() << " : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;
	    exit(-1);
	  }

	// Reset the statement for the next use
	//--------------------------------------
	l_status = sqlite3_reset(m_get_by_name_stmt);  
	if(l_status != SQLITE_OK)
	  {
	    std::cout << "ERROR during reset of " << this->get_name() << " get_by_id statement : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;     
	    exit(-1);
	  }

	// Reset bindings because they are now useless
	//--------------------------------------------
#if SQLITE_VERSION_NUMBER > 3006000
	l_status = sqlite3_clear_bindings(m_get_by_name_stmt);
	if(l_status != SQLITE_OK)
	  {
	    std::cout << "ERROR during reset of bindings of " << this->get_name() << " get_by_id statement : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;     
	    exit(-1);
	  }
#endif
	return l_result;

      }

    //------------------------------------------------------------------------------
    template <class T> void osm_cache_named_table<T>::get_by_name(const std::string & p_name,std::vector<T> &p_result,bool p_exact)
      {
	std::string l_param_value(p_exact ? p_name : "%");
	if(!p_exact)
	  {
	    l_param_value += p_name + "%";
	  }

	// Binding values to statement
	//----------------------------
	int l_status = sqlite3_bind_text(m_get_by_name_stmt,sqlite3_bind_parameter_index(m_get_by_name_stmt,"@name"),l_param_value.c_str(),-1,SQLITE_STATIC);
	if(l_status != SQLITE_OK)
	  {
	    std::cout << "ERROR during binding of name parameter for get_by_name statement of " << this->get_name() << " : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;     
	    exit(-1);
	  }
    
	// Executing statement
	//---------------------
	while( (l_status = sqlite3_step(m_get_by_name_stmt)) == SQLITE_ROW)
	  {
	    p_result.push_back(osm_cache_base_table_description<T>::get_item_from_row(m_get_by_name_stmt));
	  }
	if(l_status != SQLITE_DONE)
	  {
	    std::cout << "ERROR during selection of " << this->get_name() << " : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;
	    exit(-1);
	  }

	std::cout << "" << this->get_name() << " get_by_name successfully listed" << std::endl ;


	// Reset the statement for the next use
	//--------------------------------------
	l_status = sqlite3_reset(m_get_by_name_stmt);  
	if(l_status != SQLITE_OK)
	  {
	    std::cout << "ERROR during reset of " << this->get_name() << " get_by_name statement : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;     
	    exit(-1);
	  }

	// Reset bindings because they are now useless
	//--------------------------------------------
	l_status = sqlite3_clear_bindings(m_get_by_name_stmt);
	if(l_status != SQLITE_OK)
	  {
	    std::cout << "ERROR during reset of bindings of " << this->get_name() << " get_by_name statement : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;     
	    exit(-1);
	  }

      }

    //------------------------------------------------------------------------------
    template <class T> void osm_cache_named_table<T>::get_all(std::vector<T> & p_list)
      {

	int l_status = 0;
	// Executing statement
	//---------------------
	while( (l_status = sqlite3_step(m_get_all_ordered_by_name_stmt)) == SQLITE_ROW)
	  {
	    p_list.push_back(osm_cache_base_table_description<T>::get_item_from_row(m_get_all_ordered_by_name_stmt));
	  }
	if(l_status != SQLITE_DONE)
	  {
	    std::cout << "ERROR during selection of " << this->get_name() << " : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;
	    exit(-1);
	  }

#ifdef ENABLE_SUCCESS_STATUS_DISPLAY
	std::cout <<  this->get_name() << " successfully listed" << std::endl ;
#endif

	// Reset the statement for the next use
	//--------------------------------------
	l_status = sqlite3_reset(m_get_all_ordered_by_name_stmt);  
	if(l_status != SQLITE_OK)
	  {
	    std::cout << "ERROR during reset of " << this->get_name() << " get_all_ordered_by_name statement : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;     
	    exit(-1);
	  }

      }
}

#endif
