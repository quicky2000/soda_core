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
#ifndef _STRING_UTILITIES_H_
#define _STRING_UTILITIES_H_

#include <locale>
#include <string>

namespace osm_diff_watcher
{
  class string_utilities
  {
  public:
    inline static void narrow(const std::wstring & p_wstring, std::string & p_string);
  private:
  };

  void string_utilities::narrow(const std::wstring & p_wstring, std::string & p_string)
  {
    std::locale l_loc;
    int length = p_wstring.length();
    char * l_converted = new char [length+1];
    std::use_facet<std::ctype<wchar_t> >(l_loc).narrow ( p_wstring.c_str(), p_wstring.c_str()+length+1, '?', l_converted);
    p_string = l_converted;
  }
}
#endif // _STRING_UTILITIES_H_
//EOF
