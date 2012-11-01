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
#ifndef _DOM_SIMPLE_ANALYZER_IF_H_
#define _DOM_SIMPLE_ANALYZER_IF_H_

#include "dom_analyzer_generic_if.h"
#include "dom_tree.h"
#include "xmlParser.h"

namespace osm_diff_watcher
{
  class dom_simple_analyzer_if: public osm_diff_analyzer_dom_if::dom_analyzer_generic_if<XMLNode>
  {
  public:
    virtual void init(void)=0;
    inline ~dom_simple_analyzer_if(void){}
 };
}

#endif // _DOM_SIMPLE_ANALYZER_IF_H_
//EOF
