#ifndef _OSM_BOUNDING_BOX_H_
#define _OSM_BOUNDING_BOX_H_

namespace osm_diff_watcher
{
  class osm_bounding_box
  {
  public:
    inline osm_bounding_box(const float & p_min_lat,
                            const float & p_min_lon,
                            const float & p_max_lat,
                            const float & p_max_lon);
    inline const float & get_min_lat(void)const;
    inline const float & get_min_lon(void)const;
    inline const float & get_max_lat(void)const;
    inline const float & get_max_lon(void)const;
  private:
    float m_min_lat;
    float m_min_lon;
    float m_max_lat;
    float m_max_lon;
  };

  //----------------------------------------------------------------------------
  osm_bounding_box::osm_bounding_box(const float & p_min_lat,
                                     const float & p_min_lon,
                                     const float & p_max_lat,
                                     const float & p_max_lon):
    m_min_lat(p_min_lat),
    m_min_lon(p_min_lon),
    m_max_lat(p_max_lat),
    m_max_lon(p_max_lon)
    {
    }

  //----------------------------------------------------------------------------
  const float & osm_bounding_box::get_min_lat(void)const
    {
      return m_min_lat;
    }

  //----------------------------------------------------------------------------
  const float & osm_bounding_box::get_min_lon(void)const
    {
      return m_min_lon;
    }

  //----------------------------------------------------------------------------
  const float & osm_bounding_box::get_max_lat(void)const
    {
      return m_max_lat;
    }

  //----------------------------------------------------------------------------
  const float & osm_bounding_box::get_max_lon(void)const
    {
      return m_max_lon;
    }
}
#endif // _OSM_BOUNDING_BOX_H_
//EOF
