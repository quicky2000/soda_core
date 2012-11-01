#include "osm_api.h"
#include "download_buffer.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>

namespace osm_diff_watcher
{
  //----------------------------------------------------------------------------
  osm_api::osm_api(osm_ressources & p_ressources):
    m_ressources(p_ressources)
  {
  }

}

//EOF

