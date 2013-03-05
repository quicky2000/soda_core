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
#ifndef _OSM_CACHE_ORDERED_TABLE_H
#define _OSM_CACHE_ORDERED_TABLE_H

#include "osm_cache_base_table.h"
#include "quicky_exception.h"
#include <sstream>

namespace osm_diff_watcher
{
  class named_item;

  template <class T>
    class osm_cache_ordered_table: public osm_cache_base_table<T>
  {
  public:
    osm_cache_ordered_table(const std::string & p_name=osm_cache_base_table_description<T>::get_class_type());
    ~osm_cache_ordered_table(void);
    void set_db(sqlite3 *p_db);

    const uint32_t get_by_id_version_rank(osm_api_data_types::osm_object::t_osm_id p_id,
                                          const osm_api_data_types::osm_core_element::t_osm_version & p_version,
                                          const uint32_t & p_rank,
                                          T & p_data);
    void get_by_id_version_ordered(osm_api_data_types::osm_object::t_osm_id p_id,
                                   const osm_api_data_types::osm_core_element::t_osm_version & p_version,
                                   std::vector<T> & p_data);
  private:
    sqlite3_stmt *m_get_by_id_version_rank_stmt;
    sqlite3_stmt *m_get_by_id_version_ordered_stmt;
  };


  //------------------------------------------------------------------------------
  template <class T>
    osm_cache_ordered_table<T>::osm_cache_ordered_table(const std::string & p_name):
    osm_cache_base_table<T>(p_name),
    m_get_by_id_version_rank_stmt(NULL),
    m_get_by_id_version_ordered_stmt(NULL)
      {
      }

    //------------------------------------------------------------------------------
    template <class T>
      osm_cache_ordered_table<T>::~osm_cache_ordered_table(void)
      {
        sqlite3_finalize(m_get_by_id_version_ordered_stmt);
        sqlite3_finalize(m_get_by_id_version_rank_stmt);
#ifdef DEBUG
	std::cout << "Table " << this->get_name() << " end of destruction" << std::endl ;
#endif
      }

    //------------------------------------------------------------------------------
    template <class T>
      void osm_cache_ordered_table<T>::set_db(sqlite3 *p_db)
      {
	osm_cache_base_table<T>::set_db(p_db,false);

	// Preparing get_by_id_version_rank statements
	//--------------------------------------------
	int l_status = sqlite3_prepare_v2(osm_cache_base_table<T>::get_db(),("SELECT Id,"+osm_cache_base_table_description<T>::get_table_fields()+" FROM " + this->get_name() + " WHERE Id = $id AND Version = $version AND Position = $position").c_str(),-1,&m_get_by_id_version_rank_stmt,NULL);
	if(l_status != SQLITE_OK)
	  {
            std::stringstream l_stream;
            l_stream << "ERROR during preparation of statement to get "+osm_cache_base_table_description<T>::get_table_fields()+" item by id and version : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) ;     
	    throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
	  }

	// Preparing get_by_id_version_ordered statements
	//--------------------------------------------
	l_status = sqlite3_prepare_v2(osm_cache_base_table<T>::get_db(),("SELECT Id,"+osm_cache_base_table_description<T>::get_table_fields()+" FROM " + this->get_name() + " WHERE Id = $id AND Version = $version ORDER BY Position ASC").c_str(),-1,&m_get_by_id_version_ordered_stmt,NULL);
	if(l_status != SQLITE_OK)
	  {
            std::stringstream l_stream;
            l_stream << "ERROR during preparation of statement to get "+osm_cache_base_table_description<T>::get_table_fields()+" item by id and version : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) ;     
	    throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
	  }

#ifdef ENABLE_SUCCESS_STATUS_DISPLAY
	std::cout << "Statements OK" << std::endl ;

	std::cout << "Table " << this->get_name() << " successfully created" << std::endl ;
#endif  
      }

    //------------------------------------------------------------------------------
    template <class T>
      const uint32_t osm_cache_ordered_table<T>::get_by_id_version_rank(osm_api_data_types::osm_object::t_osm_id p_id,
                                                                        const osm_api_data_types::osm_core_element::t_osm_version & p_version,
                                                                        const uint32_t & p_rank,
                                                                        T & p_data)
      {
 
	uint32_t l_result = 0;
	// Binding values to statement
	//----------------------------

	int l_status = sqlite3_bind_int64(m_get_by_id_version_rank_stmt,sqlite3_bind_parameter_index(m_get_by_id_version_rank_stmt,"$id"),p_id);
	if(l_status != SQLITE_OK)
	  {
            std::stringstream l_stream;
            l_stream << "ERROR during binding of id parameter for get_by_id_version_rank statement of " << this->get_name() << " : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) ;     
	    throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
	  }
    
        l_status = sqlite3_bind_int(m_get_by_id_version_rank_stmt,sqlite3_bind_parameter_index(m_get_by_id_version_rank_stmt,"$version"),p_version);
	if(l_status != SQLITE_OK)
	  {
            std::stringstream l_stream;
            l_stream << "ERROR during binding of version parameter for get_by_id_version_rank statement of " << this->get_name() << " : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) ;     
	    throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
	  }
    
        l_status = sqlite3_bind_int(m_get_by_id_version_rank_stmt,sqlite3_bind_parameter_index(m_get_by_id_version_rank_stmt,"$position"),p_rank);
	if(l_status != SQLITE_OK)
	  {
            std::stringstream l_stream;
            l_stream << "ERROR during binding of position parameter for get_by_id_version_rank statement of " << this->get_name() << " : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) ;     
	    throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
	  }
    
	// Executing statement
	//---------------------
	l_status = sqlite3_step(m_get_by_id_version_rank_stmt);
	if( l_status == SQLITE_ROW)
	  {
#ifdef ENABLE_SUCCESS_STATUS_DISPLAY
	    std::cout << this->get_name() << " successfully selected by id and version" << std::endl ;
#endif
	    p_data = osm_cache_base_table_description<T>::get_item_from_row(m_get_by_id_version_rank_stmt);
	    // Ensure that ID is unique
	    l_status = sqlite3_step(m_get_by_id_version_rank_stmt);
	    if( l_status == SQLITE_DONE)
	      {
#ifdef ENABLE_SUCCESS_STATUS_DISPLAY
		std::cout << this->get_name() << " successfully selected by id and version done" << std::endl ;
#endif
                l_result = 1;
              }
	    else
	      {
                std::stringstream l_stream;
                l_stream << "ERROR during selection of " << this->get_name() << " : Id " << p_id << " is not unique " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) ;
		throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
	      }
	  }
	else if(l_status != SQLITE_DONE)
	  {
            std::stringstream l_stream;
            l_stream << "ERROR during selection of " << this->get_name() << " : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) ;
	    throw quicky_exception::quicky_runtime_exception(l_stream.str(),__LINE__,__FILE__);
	  }


	// Reset the statement for the next use
	//--------------------------------------
	l_status = sqlite3_reset(m_get_by_id_version_rank_stmt);  
	if(l_status != SQLITE_OK)
	  {
            std::stringstream l_stream;
            l_stream << "ERROR during reset of " << this->get_name() << " get_by_id_version_rank statement : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) ;     
	    throw quicky_exception::quicky_runtime_exception(l_stream.str(),__LINE__,__FILE__);
	  }

	// Reset bindings because they are now useless
	//--------------------------------------------
	l_status = sqlite3_clear_bindings(m_get_by_id_version_rank_stmt);
	if(l_status != SQLITE_OK)
	  {
            std::stringstream l_stream;
            l_stream << "ERROR during reset of bindings of " << this->get_name() << " get_by_id_version_rank statement : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) ;     
	    throw quicky_exception::quicky_runtime_exception(l_stream.str(),__LINE__,__FILE__);
	  }
        return l_result;
      }

    //------------------------------------------------------------------------------
    template <class T>
      void osm_cache_ordered_table<T>::get_by_id_version_ordered(osm_api_data_types::osm_object::t_osm_id p_id,
                                                                 const osm_api_data_types::osm_core_element::t_osm_version & p_version,
                                                                 std::vector<T> & p_result)
      {
 
	// Binding values to statement
	//----------------------------

	int l_status = sqlite3_bind_int64(m_get_by_id_version_ordered_stmt,sqlite3_bind_parameter_index(m_get_by_id_version_ordered_stmt,"$id"),p_id);
	if(l_status != SQLITE_OK)
	  {
            std::stringstream l_stream;
            l_stream << "ERROR during binding of id parameter for get_by_id_version_ordered statement of " << this->get_name() << " : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) ;     
	    throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
	  }
    
        l_status = sqlite3_bind_int(m_get_by_id_version_ordered_stmt,sqlite3_bind_parameter_index(m_get_by_id_version_ordered_stmt,"$version"),p_version);
	if(l_status != SQLITE_OK)
	  {
            std::stringstream l_stream;
            l_stream << "ERROR during binding of version parameter for get_by_id_version_ordered statement of " << this->get_name() << " : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) ;     
	    throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
	  }
        
	// Executing statement
	//---------------------
	while( (l_status = sqlite3_step(m_get_by_id_version_ordered_stmt)) == SQLITE_ROW)
	  {
	    p_result.push_back(osm_cache_base_table_description<T>::get_item_from_row(m_get_by_id_version_ordered_stmt));
          }
        if( l_status != SQLITE_DONE)
          {
            std::stringstream l_stream;
            l_stream << "ERROR during selection of " << this->get_name() << " : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) ;
	    throw quicky_exception::quicky_runtime_exception(l_stream.str(),__LINE__,__FILE__);
	  }

	// Reset the statement for the next use
	//--------------------------------------
	l_status = sqlite3_reset(m_get_by_id_version_ordered_stmt);  
	if(l_status != SQLITE_OK)
	  {
            std::stringstream l_stream;
            l_stream << "ERROR during reset of " << this->get_name() << " get_by_id_version_ordered statement : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) ;     
	    throw quicky_exception::quicky_runtime_exception(l_stream.str(),__LINE__,__FILE__);
	  }

	// Reset bindings because they are now useless
	//--------------------------------------------
	l_status = sqlite3_clear_bindings(m_get_by_id_version_ordered_stmt);
	if(l_status != SQLITE_OK)
	  {
            std::stringstream l_stream;
            l_stream << "ERROR during reset of bindings of " << this->get_name() << " get_by_id_version_ordered statement : " << sqlite3_errmsg(osm_cache_base_table<T>::get_db()) ;     
	    throw quicky_exception::quicky_runtime_exception(l_stream.str(),__LINE__,__FILE__);
	  }
      }

 
 
}

#endif // _OSM_CACHE_ORDERED_TABLE_H
//EOF
