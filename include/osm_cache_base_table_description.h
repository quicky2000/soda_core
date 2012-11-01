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
