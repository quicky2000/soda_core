#include "osm_ressources.h"

namespace osm_diff_watcher
{

  osm_ressources::osm_ressources(void):
    m_domain("openstreetmap.org"),
    m_data_domain("planet."+m_domain),
    m_redaction_domain(m_data_domain+"/redaction_period"), //Latest sequence number 229907
    m_replication_domain(m_data_domain+"/replication"),
    m_cc_by_sa_domain(m_data_domain+"cc-by-sa"), // Latest minute sequence number 1268792
    m_api("www."+m_domain+"/api"),
    m_current_api_version("0.6"),
    m_current_api(m_api+"/"+m_current_api_version)
  {
  }
  //"http://planet.openstreetmap.org/minute-replicate/"
  //"http://planet.openstreetmap.org/redaction-period/minute-replicate/"
  //"http://planet.openstreetmap.org/replication/minute/"

}
//EOF
