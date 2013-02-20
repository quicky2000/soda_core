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
#ifndef _SODA_UI_IF_H_
#define _SODA_UI_IF_H_

#include <string>

namespace osm_diff_analyzer_if
{
  class osm_diff_state;
  class analyzer_base;
}

namespace osm_diff_watcher
{
  class soda_Ui_if
  {
  public:
    // General communication
    virtual void clear_common_text()=0;
    virtual void append_common_text(const std::string & p_text)=0;
    virtual void update_diff_state(const osm_diff_analyzer_if::osm_diff_state & p_diff_state)=0;
    virtual void instantiate_module(const std::string & p_name,const osm_diff_analyzer_if::analyzer_base & p_module)=0;
    virtual void configure_progress_display(const unsigned int & p_min,const unsigned int & p_max)=0;
    virtual void update_progress(const unsigned int & p_value)=0;
    // Module communication
    virtual void append_log_text(const osm_diff_analyzer_if::analyzer_base & p_module,const std::string & p_text)=0;
    virtual void declare_html_report(const osm_diff_analyzer_if::analyzer_base & p_module,const std::string & p_name)=0;
    // Virtual destructor
    inline virtual ~soda_Ui_if(void){}
  private:
  };
}
#endif // _SODA_UI_IF_H_
//EOF
