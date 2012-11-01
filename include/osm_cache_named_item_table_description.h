#ifndef _OSM_CACHE_NAMED_ITEM_TABLE_DESCRIPTION_H_
#define _OSM_CACHE_NAMED_ITEM_TABLE_DESCRIPTION_H_

#include "osm_cache_base_table_description.h"
#include "osm_cache_named_item.h"
#include <sqlite3.h>
#include <iostream>
#include <cstdlib>

namespace osm_diff_watcher
{
  template <> 
    class osm_cache_base_table_description<osm_cache_named_item>
    {

    public:
      inline static const std::string & get_class_type(void);
      inline static const std::string & get_table_fields_declaration(void);
      inline static const std::string & get_table_fields(void);
      inline static const std::string & get_update_fields(void);
      inline static const std::string & get_field_values(void);
      inline static void bind_item_values(const osm_cache_named_item & p_name,sqlite3_stmt* p_stmt,sqlite3 *p_db);
      inline static osm_cache_named_item get_item_from_row(sqlite3_stmt* p_stmt);
    private:
      static const std::string m_class_type;
      static const std::string m_table_fields_declaration;
      static const std::string m_table_fields;
      static const std::string m_update_fields;
      static const std::string m_field_values;
    };


  //------------------------------------------------------------------------------
  const std::string & osm_cache_base_table_description<osm_cache_named_item>::get_class_type(void)
    {
      return m_class_type;
    }

  //------------------------------------------------------------------------------
  const std::string & osm_cache_base_table_description<osm_cache_named_item>::get_table_fields_declaration(void)
    {
      return m_table_fields_declaration;
    }

  //------------------------------------------------------------------------------
  const std::string & osm_cache_base_table_description<osm_cache_named_item>::get_table_fields(void)
    {
      return m_table_fields;
    }

  //------------------------------------------------------------------------------
  const std::string & osm_cache_base_table_description<osm_cache_named_item>::get_update_fields(void)
    {
      return m_update_fields;
    }

  //------------------------------------------------------------------------------
  const std::string & osm_cache_base_table_description<osm_cache_named_item>::get_field_values(void)
    {
      return m_field_values;
    }

  //------------------------------------------------------------------------------
  void osm_cache_base_table_description<osm_cache_named_item>::bind_item_values(const osm_cache_named_item & p_named_item,sqlite3_stmt* p_stmt,sqlite3 *p_db)
  {
    int l_status = sqlite3_bind_text(p_stmt,sqlite3_bind_parameter_index(p_stmt,"$name"),p_named_item.get_name().c_str(),-1,SQLITE_STATIC);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during binding of name parameter for update statement of " << get_class_type() << " : " << sqlite3_errmsg(p_db) << std::endl ;     
	exit(-1);
      }  
  }

  //------------------------------------------------------------------------------
  osm_cache_named_item osm_cache_base_table_description<osm_cache_named_item>::get_item_from_row(sqlite3_stmt* p_stmt)
    {
      return osm_cache_named_item(sqlite3_column_int64(p_stmt,0),(const char*)sqlite3_column_text(p_stmt,1));
    }
}
#endif // _OSM_CACHE_NAMED_ITEM_TABLE_DESCRIPTION_H_
//EOF
