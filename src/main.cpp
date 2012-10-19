#include "osm_diff_watcher.h"
#include <iostream>
#include <stdlib.h>

#include "osm_api.h"
#include "osm_node.h"

int main(int p_argc,char ** p_argv)
{
  std::cout << "Nb arguments : " << p_argc << std::endl ;
  uint64_t l_start_seq_number = 0;
  if(p_argc == 2)
    {
      l_start_seq_number = atoll(p_argv[1]);
    }

  osm_diff_watcher::osm_diff_watcher l_watcher;
  l_watcher.run(l_start_seq_number);

}
//EOF
