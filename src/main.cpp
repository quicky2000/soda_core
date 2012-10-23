#include "osm_diff_watcher.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "osm_api.h"
#include "osm_node.h"

int main(int p_argc,char ** p_argv)
{
  std::cout << "Nb arguments : " << p_argc << std::endl ;
  uint64_t l_start_seq_number = 0;
  std::string l_file_name("");
  if(p_argc > 2)
    {
      std::cout << "ERROR : correct usage is osm_diff_watcher.exe [configuration_file_name]" << std::endl ;
      exit(-1);
    }
  if(p_argc == 2)
    {
      l_file_name = p_argv[1];
    }

  osm_diff_watcher::osm_diff_watcher l_watcher(l_file_name);
  l_watcher.run(l_start_seq_number);

}
//EOF
