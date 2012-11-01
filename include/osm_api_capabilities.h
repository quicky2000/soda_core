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
#ifndef _OSM_API_CAPABILITIES_H_
#define _OSM_API_CAPABILITIES_H_

namespace osm_diff_watcher
{
  class osm_api_capabilities
  {
    friend std::ostream & operator<<(std::ostream & p_stream,const osm_api_capabilities & p_capabilities);
  public:
    inline osm_api_capabilities(const std::string & p_version_min,
                                const std::string & p_version_max,
                                const std::string & p_area_maximum,
                                const uint64_t & p_tracepoints_per_page,
                                const uint64_t & p_waynodes_max,
                                const uint64_t & p_changeset_max_elements,
                                const uint32_t & p_timeout_max);
    inline const std::string & get_version_min(void)const;
    inline const std::string & get_version_max(void)const;
  private:
    const std::string m_version_min;
    const std::string m_version_max;
    const std::string m_area_maximum;
    const uint64_t m_tracepoints_per_page;
    const uint64_t m_waynodes_max;
    const uint64_t m_changeset_max_elements;
    const uint32_t m_timeout_max;
  };

  //------------------------------------------------------------------------------
  inline std::ostream & operator<<(std::ostream & p_stream,const osm_api_capabilities & p_capabilities)
    {
      p_stream << "version min=" << p_capabilities.m_version_min << " max=" << p_capabilities.m_version_max << std::endl ;
      p_stream << "area max=" << p_capabilities.m_area_maximum << std::endl ;
      p_stream << "tracepoints per_page=" << p_capabilities.m_tracepoints_per_page << std::endl ;
      p_stream << "waynodes max=" << p_capabilities.m_waynodes_max << std::endl ;
      p_stream << "changeset max_elements=" << p_capabilities.m_changeset_max_elements << std::endl ;
      p_stream << "timeout max=" << p_capabilities.m_timeout_max << std::endl ;
      return p_stream;
    }

  //----------------------------------------------------------------------------
  osm_api_capabilities::osm_api_capabilities(const std::string & p_version_min,
                                             const std::string & p_version_max,
                                             const std::string & p_area_maximum,
                                             const uint64_t & p_tracepoints_per_page,
                                             const uint64_t & p_waynodes_max,
                                             const uint64_t & p_changeset_max_elements,
                                             const uint32_t & p_timeout_max):
    m_version_min(p_version_min),
    m_version_max(p_version_max),
    m_area_maximum(p_area_maximum),
    m_tracepoints_per_page(p_tracepoints_per_page),
    m_waynodes_max(p_waynodes_max),
    m_changeset_max_elements(p_changeset_max_elements),
    m_timeout_max(p_timeout_max)
    {
    }

  //----------------------------------------------------------------------------
    const std::string & osm_api_capabilities::get_version_min(void)const
      {
        return m_version_min;
      }

  //----------------------------------------------------------------------------
    const std::string & osm_api_capabilities::get_version_max(void)const
      {
        return m_version_max;
      }
}
#endif // _OSM_API_CAPABILITIES_H_
//EOF
