#ifndef _OSM_API_H_
#define _OSM_API_H_

#include "osm_core_element.h"
#include "osm_ressources.h"
#include "url_reader.h"
#include "osm_api_capabilities.h"
#include "dom_osm_parser.h"
#include "osm_diff_dom_parser.h"
#include "osm_change.h"
#include "dom_osm_change_extractor.h"
#include <string>
#include <sstream>

//------------------------------------------------------------------------------
// Taken from http://wiki.openstreetmap.org/wiki/API_v0.6_%28Archive%29#Changes_in_related_software
//
// GET 	capabilities                    Returns server capabilities.                                 Done
// GET 	node/#id                        Returns the XML for that node.                               Done
// GET 	node/#id/history                Returns all versions of the node.                            Done
// GET 	node/#id/#version               Returns the XML for that version of the node.                Done
// GET 	node/#id/ways 	                Returns the XML for all ways that this node is part of.      Done
// GET 	node/#id/relations              Returns the XML for all relations that this node is part of. Done
// GET 	nodes?nodes=#id,#id,...         Returns the XML for all given node numbers. 	             Done
// GET 	way/#id                         Returns the XML for that way.                                Done
// GET 	way/#id/history 	        Returns all versions of the way.                             Done
// GET 	way/#id/#version 	        Returns the XML for that version of the way.                 Done
// GET 	way/#id/relations 	        Returns the XML of all relations that this way is part of.   Done
// GET 	way/#id/full                    Returns XML of a way and all its nodes.                      Done
// GET 	ways?ways=#id,#id,... 	        Returns XML of all numbered ways.                            Done
// GET 	relation/#id                    Returns the XML for that relation.                           Done
// GET 	relation/#id/history 	        Returns all versions of the relation.                        Done
// GET 	relation/#id/#version 	        Returns the XML for that version of the relation.            Done
// GET 	relation/#id/relations 	        Returns all relations that this relation appears in.         Done (Not tested)
// GET 	relation/#id/full 	        Returns all ways and nodes in this relation and relations    Done
//                                      directly members of this relation. 	
// GET 	relations?relations=#id,#id,... Returns the numbered relations.                              Done
// GET 	changeset/#id 	                Returns the XML for that changeset.                          Done
// GET 	changeset/#id/download 	        Downloads all the changed elements in a changeset in         Done
//                                      OsmChange format.
// GET 	changesets                      Queries changesets on bounding box, user or time range.      Done
// GET 	map 	                        Gets all the way, nodes and relations inside a bounding box  Done
// GET 	trackpoints                     Gets paginated trackpoints within a bounding box. 	     Not implemented
// GET 	changes                         Returns all changes within a given time period.              Not documented on wiki
//------------------------------------------------------------------------------

namespace osm_diff_watcher
{
  class osm_api
  {
  public:
    osm_api(osm_ressources & p_ressources);

    template <class T>
      const T * const get(const osm_api_data_types::osm_object::t_osm_id & p_id,
                          const osm_api_data_types::osm_core_element::t_osm_version & p_version=0);
    template <class T>
      const std::vector<T*> * const get_history(const osm_api_data_types::osm_object::t_osm_id & p_id);

    template <class T,class T_RETURN>
      const std::vector<T_RETURN *> * const get(const osm_api_data_types::osm_object::t_osm_id & p_id);

    template <class T>
      const std::vector<T*> * const get(const std::vector<osm_api_data_types::osm_object::t_osm_id> & p_ids);

    inline const osm_api_data_types::osm_way * const get_full_way(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                                  std::vector<osm_api_data_types::osm_node*> & p_nodes);

    inline const osm_api_data_types::osm_relation * const get_full_relation(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                                            std::vector<osm_api_data_types::osm_node*> & p_nodes,
                                                                            std::vector<osm_api_data_types::osm_way*> & p_ways);

    inline const std::vector<osm_api_data_types::osm_change*> * const get_changeset_content(const osm_api_data_types::osm_object::t_osm_id & p_id);

    inline const std::vector<osm_api_data_types::osm_changeset* > * const get_changesets(const osm_api_data_types::osm_bounding_box & p_bounding_box,
                                                                                         const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                                                         const std::string & p_user_name,
                                                                                         const std::string & p_time1,
                                                                                         const std::string & p_time2,
                                                                                         bool p_open,
                                                                                         bool p_close);

    inline void get_map(const osm_api_data_types::osm_bounding_box & p_bounding_box,
                        std::vector<osm_api_data_types::osm_node*> & p_nodes,
                        std::vector<osm_api_data_types::osm_way*> & p_ways,
                        std::vector<osm_api_data_types::osm_relation*> & p_relations);

    inline const osm_api_capabilities * get_capabilities(void);

    // Some utilities
    //TO DELETE    void dump_url(const std::string & p_url,std::string & p_result);
    //TO DELETE    void dump_url(const std::string & p_url, const std::string & p_file_name);

  private:
    dom_osm_parser m_osm_parser;
    osm_ressources & m_ressources;
    quicky_url_reader::url_reader m_url_reader;
  };

  //----------------------------------------------------------------------------
  template <class T>
    const T * const osm_api::get(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                 const osm_api_data_types::osm_core_element::t_osm_version & p_version)
    {
      std::stringstream l_id_str;
      l_id_str << p_id;
      std::string l_url = m_ressources.get_api_url()+"/"+T::get_type_str()+"/" + l_id_str.str();
      if(p_version)
        {
          std::stringstream l_version_str;
          l_version_str << p_version;
          l_url += "/"+l_version_str.str();
        }
      std::string l_result;
      m_url_reader.dump_url(l_url,l_result);
      return m_osm_parser.get<T>(l_result);     
    }

  //----------------------------------------------------------------------------
  template <class T>
    const std::vector<T*> * const osm_api::get_history(const osm_api_data_types::osm_object::t_osm_id & p_id)
    {
      std::stringstream l_id_str;
      l_id_str << p_id;
      std::string l_url = m_ressources.get_api_url()+"/"+T::get_type_str()+"/" + l_id_str.str() + "/history";
      std::string l_result;
      m_url_reader.dump_url(l_url,l_result);
      return m_osm_parser.get_list<T>(l_result);     
    }

  //----------------------------------------------------------------------------
  template <class T>
    const std::vector<T*> * const osm_api::get(const std::vector<osm_api_data_types::osm_object::t_osm_id> & p_ids)
    {
      std::string l_url = m_ressources.get_api_url()+"/"+T::get_type_str()+"s?" + T::get_type_str() + "s=";
      for(std::vector<osm_api_data_types::osm_object::t_osm_id>::const_iterator l_iter = p_ids.begin();
	  l_iter != p_ids.end();
	  ++l_iter)
	{
	  if(l_iter != p_ids.begin())
	    {
	      l_url += ",";
	    }
	  std::stringstream l_id_str;
	  l_id_str << *l_iter;
	  l_url += l_id_str.str();
	}
      std::string l_result;
      m_url_reader.dump_url(l_url,l_result);
      return m_osm_parser.get_list<T>(l_result);     
    }

  //----------------------------------------------------------------------------
  template <class T,class T_RETURN>
    const std::vector<T_RETURN *> * const osm_api::get(const osm_api_data_types::osm_object::t_osm_id & p_id)
  {
    std::stringstream l_id_str;
    l_id_str << p_id;
    std::string l_url = m_ressources.get_api_url()+"/"+T::get_type_str()+"/" + l_id_str.str() + "/" + T_RETURN::get_type_str() + "s";
    std::string l_result;
    m_url_reader.dump_url(l_url,l_result);
    return m_osm_parser.get_list<T_RETURN>(l_result);     
  }

  //----------------------------------------------------------------------------
  const osm_api_data_types::osm_way * const osm_api::get_full_way(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                                  std::vector<osm_api_data_types::osm_node*> & p_nodes)
    {
      std::stringstream l_id_str;
      l_id_str << p_id;
      std::string l_url = m_ressources.get_api_url()+"/way/" + l_id_str.str() + "/full";
      std::string l_result;
      m_url_reader.dump_url(l_url,l_result);
      return m_osm_parser.get_full_way(l_result,p_nodes);           
    }

  //----------------------------------------------------------------------------
  const osm_api_data_types::osm_relation * const osm_api::get_full_relation(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                                            std::vector<osm_api_data_types::osm_node*> & p_nodes,
                                                                            std::vector<osm_api_data_types::osm_way*> & p_ways)
    {
      std::stringstream l_id_str;
      l_id_str << p_id;
      std::string l_url = m_ressources.get_api_url()+"/relation/" + l_id_str.str() + "/full";
      std::string l_result;
      m_url_reader.dump_url(l_url,l_result);
      return m_osm_parser.get_full_relation(l_result,p_nodes,p_ways);           
    }

  //----------------------------------------------------------------------------
  const std::vector<osm_api_data_types::osm_change*> * const osm_api::get_changeset_content(const osm_api_data_types::osm_object::t_osm_id & p_id)
    {
      
      std::stringstream l_id_str;
      l_id_str << p_id;
      std::string l_url = m_ressources.get_api_url()+"/changeset/" + l_id_str.str() + "/download";
      std::string l_result;
      m_url_reader.dump_url(l_url,l_result);
      dom_parser l_osm_change_parser("osmChange");
      dom_osm_change_extractor l_extractor;
      l_osm_change_parser.add_analyzer(l_extractor);
      l_osm_change_parser.parse_string(l_result);
      return l_extractor.get_result();           
    }

  //----------------------------------------------------------------------------
  const std::vector<osm_api_data_types::osm_changeset* > * const osm_api::get_changesets(const osm_api_data_types::osm_bounding_box & p_bounding_box,
                                                                                         const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                                                         const std::string & p_user_name,
                                                                                         const std::string & p_time1,
                                                                                         const std::string & p_time2,
                                                                                         bool p_open,
                                                                                         bool p_close)
    {
      // Check arguments
      if(p_id != 0 && p_user_name != "")
        {
          std::cout << "ERROR : it is forbidden to provide both user id (" << p_id << " and user name (\"" << p_user_name << "\"" << std::endl ;
          exit(-1);
        }
      if(p_time1 == "" && p_time2 != "")
        {
          std::cout << "ERROR : time1 empty whereas time2 is not empty" << std::endl ;
          exit(-1);
        }
       
      // End of argument check

      // Building URL"
      std::string l_url = m_ressources.get_api_url()+"/" + osm_api_data_types::osm_changeset::get_type_str()+"s?";
      bool l_first = true;
      if(!p_bounding_box.is_empty())
        {
          l_url += "bbox=";
          std::stringstream l_stream ;
          l_stream << p_bounding_box.get_min_lat() << ",";
          l_stream << p_bounding_box.get_min_lon() << ",";
          l_stream << p_bounding_box.get_max_lat() << ",";
          l_stream << p_bounding_box.get_max_lon();
          l_url += l_stream.str();
          l_first = false;
        }
      if(p_id)
        {
          if(!l_first) l_url +="&";
          std::stringstream l_stream;
          l_stream << p_id;
          l_url += "user=" + l_stream.str();
          l_first = false;          
        }
      if(p_user_name!= "")
        {
          if(!l_first) l_url +="&";
          l_url += "display_name=" + p_user_name;
          l_first = false;
        }
      if(p_time1 != "")
        {
          if(!l_first) l_url +="&";
          l_url += "time=" + p_time1;
          l_first = false;
          if(p_time2 != "")
            {
              l_url += "," + p_time2;
            }
        }
      if(p_open)
        {
          if(!l_first) l_url +="&";
          l_url +="open";
          l_first = false;
        }
      if(p_close)
        {
          if(!l_first) l_url +="&";
          l_url +="close";
          l_first = false;
        }
      
      std::string l_result;
      m_url_reader.dump_url(l_url,l_result);
      return m_osm_parser.get_list<osm_api_data_types::osm_changeset>(l_result);     
    }
  //----------------------------------------------------------------------------
  void osm_api::get_map(const osm_api_data_types::osm_bounding_box & p_bounding_box,
                        std::vector<osm_api_data_types::osm_node*> & p_nodes,
                        std::vector<osm_api_data_types::osm_way*> & p_ways,
                        std::vector<osm_api_data_types::osm_relation*> & p_relations)
  {
    // Building URL"
    std::string l_url = m_ressources.get_api_url()+"/map?bbox=";
    std::stringstream l_stream ;
    l_stream << p_bounding_box.get_min_lat() << ",";
    l_stream << p_bounding_box.get_min_lon() << ",";
    l_stream << p_bounding_box.get_max_lat() << ",";
    l_stream << p_bounding_box.get_max_lon();
    l_url += l_stream.str();
    std::string l_result;
    m_url_reader.dump_url(l_url,l_result);
    m_osm_parser.get(l_result,p_nodes,p_ways,p_relations); 
  }

  //----------------------------------------------------------------------------
  const osm_api_capabilities * osm_api::get_capabilities(void)
    {
      std::string l_url = m_ressources.get_api_url()+"/capabilities";
      std::string l_result;
      m_url_reader.dump_url(l_url,l_result);
      return m_osm_parser.get_capabilities(l_result);
    }
}
 

#endif // _OSM_API_H_
//EOF
