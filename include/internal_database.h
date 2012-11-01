#ifndef _INTERNAL_DATABASE_H_
#define _INTERNAL_DATABASE_H_

#include <string>

class sqlite3;
class sqlite3_stmt;

namespace osm_diff_watcher
{
  class internal_database
  {
  public:
    internal_database(const std::string & p_name="osm_diff_watcher.sqlite3");
  private:
    sqlite3 * m_db;
  };
}
#endif // _INTERNAL_DATABASE_H_
//EOF
