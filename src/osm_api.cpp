#include "osm_api.h"
#include "download_buffer.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>

namespace osm_diff_watcher
{
  //----------------------------------------------------------------------------
  osm_api::osm_api(void)
  {
  }

  //------------------------------------------------------------------------------
  void osm_api::dump_url(const std::string & p_url,std::string & p_result)
  {
    quicky_url_reader::download_buffer l_buffer;
    m_url_reader.read_url(p_url.c_str(),l_buffer);
    p_result = std::string(l_buffer.get_data(),l_buffer.get_size());
  }

  //------------------------------------------------------------------------------
  void osm_api::dump_url(const std::string & p_url,const std::string & p_file_name)
  {
    quicky_url_reader::download_buffer l_buffer;
    m_url_reader.read_url(p_url.c_str(),l_buffer);
    std::ofstream l_output_file(p_file_name.c_str());
    if(l_output_file==NULL)
      {
	std::cout << "ERROR : Unable to open output file \"tmp_diff.gz\"" << std::endl ;
	exit(EXIT_FAILURE);
      }
    l_output_file.write(l_buffer.get_data(),l_buffer.get_size());
    l_output_file.close();
  }

}

//EOF

