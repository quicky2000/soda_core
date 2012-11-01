#ifndef _OSM_RESSOURCES_H_
#define _OSM_RESSOURCES_H_

#include <string>
#include "url_reader.h"
#include "download_buffer.h"


namespace osm_diff_analyzer_if
{
  class osm_diff_state;
}

namespace osm_diff_watcher
{

  class osm_ressources
  {
  public:
    inline const std::string & get_api_url(void)const;
    void get_user_subscription_date(const std::string & p_user_name,std::string & p_date)const;
    const std::string get_url_minute_diff(const uint64_t & p_seq_number)const;
    const osm_diff_analyzer_if::osm_diff_state * get_minute_diff_state(void)const;
    static osm_ressources & instance(void);
    static void remove_instance(void);
  private:
    osm_ressources(void);

    const std::string m_domain;
    const std::string m_data_domain;
    const std::string m_redaction_domain;
    const std::string m_replication_domain;
    const std::string m_cc_by_sa_domain;
    const std::string m_api;
    const std::string m_current_api_version;
    const std::string m_current_api;
    static osm_ressources * m_instance;
  }
  ;

  //----------------------------------------------------------------------------
  const std::string & osm_ressources::get_api_url(void)const
  {
    return m_current_api;
  }

}
#endif // _OSM_RESSOURCES_H_
//EOF
