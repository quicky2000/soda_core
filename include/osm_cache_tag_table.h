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
#ifndef _OSM_CACHE_TAG_TABLE_H_
#define _OSM_CACHE_TAG_TABLE_H_
#include "osm_cache_versionned_table.h"
#include "osm_cache_object_tag.h"

namespace osm_diff_watcher
{
  template <class T>
  class osm_cache_tag_table: public osm_cache_versionned_table<T>
  {
  public:
    osm_cache_tag_table(const std::string & p_name=osm_cache_base_table_description<T>::get_class_type());
    ~osm_cache_tag_table(void);
    void set_db(sqlite3 *p_db);

    bool exists(const osm_cache_object_tag & p_tag);
  private:
    sqlite3_stmt *m_exists_stmt;   
  };

   //------------------------------------------------------------------------------
  template <class T>
    osm_cache_tag_table<T>::osm_cache_tag_table(const std::string & p_name):
    osm_cache_versionned_table<T>(p_name),
    m_exists_stmt(NULL)
      {
      }

    //------------------------------------------------------------------------------
    template <class T>
      osm_cache_tag_table<T>::~osm_cache_tag_table(void)
      {
        sqlite3_finalize(m_exists_stmt);
	std::cout << "Table " << this->get_name() << " end of destruction" << std::endl ;
      }

    //------------------------------------------------------------------------------
    template <class T>
      void osm_cache_tag_table<T>::set_db(sqlite3 *p_db)
      {
	osm_cache_versionned_table<T>::set_db(p_db);

	// Preparing exists statements
	//--------------------------------------------
	int l_status = sqlite3_prepare_v2(osm_cache_base_table<T>::get_db(),("SELECT Id,"+osm_cache_base_table_description<T>::get_table_fields()+" FROM " + this->get_name() + " WHERE Id = $id AND Version = $version AND Tag_Name_id = $tag_name_id AND Tag_Value_Id = $tag_value_id").c_str(),-1,&m_exists_stmt,NULL);
	if(l_status != SQLITE_OK)
	  {
	    std::cout << "ERROR during preparation of statement to check "+osm_cache_base_table_description<T>::get_table_fields()+" existence : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;     
	    exit(-1);
	  }

#ifdef ENABLE_SUCCESS_STATUS_DISPLAY
	std::cout << "Statements OK" << std::endl ;

	std::cout << "Table " << this->get_name() << " successfully created" << std::endl ;
#endif  
      }

    //------------------------------------------------------------------------------
    template <class T>
      bool osm_cache_tag_table<T>::exists(const osm_cache_object_tag & p_tag)
      {
 
	bool l_result = false;
	// Binding values to statement
	//----------------------------
	int l_status = sqlite3_bind_int64(m_exists_stmt,sqlite3_bind_parameter_index(m_exists_stmt,"$id"),p_tag.get_id());
	if(l_status != SQLITE_OK)
	  {
	    std::cout << "ERROR during binding of id parameter for exists statement of " << this->get_name() << " : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;     
	    exit(-1);
	  }
    
        l_status = sqlite3_bind_int(m_exists_stmt,sqlite3_bind_parameter_index(m_exists_stmt,"$version"),p_tag.get_version());
	if(l_status != SQLITE_OK)
	  {
	    std::cout << "ERROR during binding of version parameter for exists statement of " << this->get_name() << " : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;     
	    exit(-1);
	  }
    
	l_status = sqlite3_bind_int64(m_exists_stmt,sqlite3_bind_parameter_index(m_exists_stmt,"$tag_name_id"),p_tag.get_tag_name_id());
	if(l_status != SQLITE_OK)
	  {
	    std::cout << "ERROR during binding of tag_name_id parameter for exists statement of " << this->get_name() << " : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;     
	    exit(-1);
	  }
	l_status = sqlite3_bind_int64(m_exists_stmt,sqlite3_bind_parameter_index(m_exists_stmt,"$tag_value_id"),p_tag.get_tag_value_id());
	if(l_status != SQLITE_OK)
	  {
	    std::cout << "ERROR during binding of tag_value_id parameter for exists statement of " << this->get_name() << " : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;     
	    exit(-1);
	  }
    
	// Executing statement
	//---------------------
	l_status = sqlite3_step(m_exists_stmt);
	if( l_status == SQLITE_ROW)
	  {
#ifdef ENABLE_SUCCESS_STATUS_DISPLAY
	    std::cout << this->get_name() << " successfully selected by id and version" << std::endl ;
#endif
	    // Ensure that ID is unique
	    l_status = sqlite3_step(m_exists_stmt);
	    if( l_status == SQLITE_DONE)
	      {
#ifdef ENABLE_SUCCESS_STATUS_DISPLAY
		std::cout << this->get_name() << " successfully selected by id and version done" << std::endl ;
#endif
		l_result = true;
              }
	    else
	      {
		std::cout << "ERROR during selection of " << this->get_name() << " : tag is not unique " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;
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
	l_status = sqlite3_reset(m_exists_stmt);  
	if(l_status != SQLITE_OK)
	  {
	    std::cout << "ERROR during reset of " << this->get_name() << " exists statement : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;     
	    exit(-1);
	  }

	// Reset bindings because they are now useless
	//--------------------------------------------
	l_status = sqlite3_clear_bindings(m_exists_stmt);
	if(l_status != SQLITE_OK)
	  {
	    std::cout << "ERROR during reset of bindings of " << this->get_name() << " exists statement : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) << std::endl ;     
	    exit(-1);
	  }
        return l_result;
      }


 
}

#endif // _OSM_CACHE_TAG_TABLE_H_
//EOF
