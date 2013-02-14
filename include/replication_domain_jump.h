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
#ifndef _REPLICATION_DOMAIN_JUMP_H_
#define _REPLICATION_DOMAIN_JUMP_H_

#include <inttypes.h>
#include <string>

namespace osm_diff_watcher
{
  class replication_domain_jump
  {
  public:
    inline replication_domain_jump(const uint64_t & p_id,
                                   const std::string & p_domain,
                                   const uint64_t & p_id2,
                                   const std::string & p_domain2);
    inline const uint64_t & get_old_id(void)const;
    inline const uint64_t & get_new_id(void)const;
    inline const std::string & get_old_domain(void)const;
    inline const std::string & get_new_domain(void)const;
  private:
    uint64_t m_id;
    std::string m_domain;
    uint64_t m_id2;
    std::string m_domain2;
  };

  //----------------------------------------------------------------------------
  replication_domain_jump::replication_domain_jump(const uint64_t & p_id,
                                                   const std::string & p_domain,
                                                   const uint64_t & p_id2,
                                                   const std::string & p_domain2):
    m_id(p_id),
    m_domain(p_domain),
    m_id2(p_id2),
    m_domain2(p_domain2)
      {
      }

    //----------------------------------------------------------------------------
    const uint64_t & replication_domain_jump::get_old_id(void)const
      {
        return m_id;
      }

    //----------------------------------------------------------------------------
    const uint64_t & replication_domain_jump::get_new_id(void)const
      {
        return m_id2;
      }
    //----------------------------------------------------------------------------
    const std::string & replication_domain_jump::get_old_domain(void)const
      {
        return m_domain;
      }
    //----------------------------------------------------------------------------
    const std::string & replication_domain_jump::get_new_domain(void)const
      {
        return m_domain2;
      }

}

#endif // _REPLICATION_DOMAIN_JUMP_H_
//EOF
