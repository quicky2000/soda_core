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
#ifndef _OUTPUT_PRODUCER_IF_
#define _OUTPUT_PRODUCER_IF_

#include "analyze_operation.h"
#include <set>
#include <iostream>

namespace osm_diff_watcher
{
  template <class T>
    class output_producer_if
    {
    public:
      void add_analyzer(T & p_analyzer);
      size_t get_nb_analyzers(void)const;
    protected:
      void perform_analyze(const analyze_operation<T> & f);
    private:
      std::set<T*> m_analyzers;
  };

  //------------------------------------------------------------------------------
  template <class T>
    void output_producer_if<T>::add_analyzer(T & p_analyzer)
    {
      m_analyzers.insert(&p_analyzer);
    }

  //------------------------------------------------------------------------------
  template <class T>
    void output_producer_if<T>::perform_analyze(const analyze_operation<T> & func)
    {
      for(typename std::set<T*>::iterator l_iter = m_analyzers.begin();l_iter != m_analyzers.end() ; ++l_iter)
	{
	  func(*l_iter);
	}
    }
  //------------------------------------------------------------------------------
  template <class T>
    size_t output_producer_if<T>::get_nb_analyzers(void)const
    {
      return  m_analyzers.size();
    }

}


#endif // _OUTPUT_PRODUCER_IF_
//EOF
