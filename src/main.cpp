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
