#ifndef _DOM_UTILITIES_H_
#define _DOM_UTILITIES_H_

#include "osm_node.h"
#include "osm_way.h"
#include "osm_relation.h"
#include "osm_changeset.h"
#include "xmlParser.h"
#include <cstdlib>
#include <cstring>

namespace osm_diff_watcher
{
  template <class T>
    class generic_dom_utilities
    {
    public:
      template <class OSM_OBJ>
        static OSM_OBJ * extract_info(const T & p_node,bool p_search_visible=false);  

      static void extract_tag_info(const T & p_node, osm_object & p_object);
      static void extract_node_ref_info(const T & p_node, osm_way & p_way);
      static void extract_member_info(const T & p_node, osm_relation & p_relation);
      static void extract_core_element_attributes(const T & p_node,
                                                  osm_object::t_osm_id & p_id,
                                                  osm_core_element::t_osm_version & p_version,
                                                  std::string & p_timestamp,
                                                  std::string & p_user,
                                                  osm_object::t_osm_id & p_uid,
                                                  osm_core_element::t_osm_id & p_changeset,
                                                  bool & p_visible,
                                                  bool p_search_visible);
    private:
    };

  template <>
    class generic_dom_utilities<XMLNode>
    {
    public:
      template <class OSM_OBJ>
        inline static OSM_OBJ * extract_info(const XMLNode & p_node,bool p_search_visible=false);  

      inline static void extract_tag_info(const XMLNode & p_node, osm_object & p_object);
      inline static void extract_node_ref_info(const XMLNode & p_node, osm_way & p_way);
      inline static void extract_member_info(const XMLNode & p_node, osm_relation & p_relation);
      inline static void extract_core_element_attributes(const XMLNode & p_node,
                                                         osm_object::t_osm_id & p_id,
                                                         osm_core_element::t_osm_version & p_version,
                                                         std::string & p_timestamp,
                                                         std::string & p_user,
                                                         osm_object::t_osm_id & p_uid,
                                                         osm_core_element::t_osm_id & p_changeset,
                                                         bool & p_visible,
                                                         bool p_search_visible);
    private:
    };

  //----------------------------------------------------------------------------
  template <class OSM_OBJ>
    OSM_OBJ * generic_dom_utilities<XMLNode>::extract_info(const XMLNode & p_node,bool p_search_visible)
    {
      // Dummy implementation should not be called
      std::cout << "No implementation provided for type \"" << OSM_OBJ::get_type_str() << "\"" << std::endl ;
      exit(-1);
      return NULL;
    }


  //----------------------------------------------------------------------------
  template <>
    inline osm_node * generic_dom_utilities<XMLNode>::extract_info<osm_node>(const XMLNode & p_node,bool p_search_visible)
    {
      osm_object::t_osm_id l_id;
      osm_core_element::t_osm_version l_version;
      std::string l_timestamp;
      std::string l_user;
      osm_object::t_osm_id l_uid;
      osm_core_element::t_osm_id l_changeset;
      bool l_visible;

      extract_core_element_attributes(p_node,l_id,l_version,l_timestamp,l_user,l_uid,l_changeset,l_visible,p_search_visible);

      // Get node lat
      XMLCSTR l_lat_str = (l_visible ? p_node.getAttribute("lat") : "0.0");
      assert(l_lat_str);
      float l_lat = atof(l_lat_str); 
      // Get node lont
      XMLCSTR l_lon_str = (l_visible ? p_node.getAttribute("lon") : "0.0");
      assert(l_lon_str);
      float l_lon = atof(l_lon_str); 

      osm_node * l_osm_node = new osm_node(l_id,l_lat,l_lon,l_timestamp,l_version,l_changeset,l_uid,l_user);
      if(l_visible)
	{
	  int l_nb_child_object = p_node.nChildNode();
	  for(int l_index = 0 ; l_index < l_nb_child_object ; ++l_index)
	    {
	      extract_tag_info(p_node.getChildNode(l_index),*l_osm_node);
	    }
	}
      return l_osm_node;
    }

  //----------------------------------------------------------------------------
  template <>
    inline osm_way * generic_dom_utilities<XMLNode>::extract_info(const XMLNode & p_node,bool p_search_visible)
    {
      osm_object::t_osm_id l_id;
      osm_core_element::t_osm_version l_version;
      std::string l_timestamp;
      std::string l_user;
      osm_object::t_osm_id l_uid;
      osm_core_element::t_osm_id l_changeset;
      bool l_visible;

      extract_core_element_attributes(p_node,l_id,l_version,l_timestamp,l_user,l_uid,l_changeset,l_visible,p_search_visible);

      osm_way * l_osm_way = new osm_way(l_id,l_timestamp,l_version,l_changeset,l_uid,l_user);
      if(l_visible)
      	{
      	  int l_nb_child_object = p_node.nChildNode();
      	  if(l_nb_child_object)
      	    {
      	      int l_index = 0;
	      
      	      while(l_index < l_nb_child_object)
      		{
      		  const XMLNode & l_node = p_node.getChildNode(l_index);
		  
      		  if(!strcmp(l_node.getName(),"nd"))
      		    {
      		      extract_node_ref_info(l_node,*l_osm_way);
      		    }
      		  else if(!strcmp(l_node.getName(),"tag"))
      		    {
      		      extract_tag_info(l_node,*l_osm_way);
      		    }
      		  ++l_index;
      		}
      	    }
      	}
      return l_osm_way;
    }

  //----------------------------------------------------------------------------
  template <>
    inline osm_relation * generic_dom_utilities<XMLNode>::extract_info(const XMLNode & p_node,bool p_search_visible)
    {
      osm_object::t_osm_id l_id;
      osm_core_element::t_osm_version l_version;
      std::string l_timestamp;
      std::string l_user;
      osm_object::t_osm_id l_uid;
      osm_core_element::t_osm_id l_changeset;
      bool l_visible;

      extract_core_element_attributes(p_node,l_id,l_version,l_timestamp,l_user,l_uid,l_changeset,l_visible,p_search_visible);

      osm_relation * l_osm_relation = new osm_relation(l_id,l_timestamp,l_version,l_changeset,l_uid,l_user);
      if(l_visible)
	{
	  int l_nb_child_object = p_node.nChildNode();
	  if(l_nb_child_object)
	    {
	      int l_index = 0;
	      
	      while(l_index < l_nb_child_object)
		{
		  const XMLNode & l_node = p_node.getChildNode(l_index);
		  
		  if(!strcmp(l_node.getName(),"member"))
		    {
		      extract_member_info(l_node,*l_osm_relation);
		    }
		  else if(!strcmp(l_node.getName(),"tag"))
		    {
		      extract_tag_info(l_node,*l_osm_relation);
		    }
		  ++l_index;
		}
	    }
	}
      return l_osm_relation;
    }


  //----------------------------------------------------------------------------
  void generic_dom_utilities<XMLNode>::extract_tag_info(const XMLNode & p_node, osm_object & p_object)
  {
    assert(!strcmp(p_node.getName(),"tag"));
    int l_nb_attribute = p_node.nAttribute();
    assert(l_nb_attribute >= 2);
    XMLCSTR l_key_str = p_node.getAttribute("k");
    assert(l_key_str);
    XMLCSTR l_value_str = p_node.getAttribute("v",0);
    assert(l_value_str);
    p_object.add_tag(l_key_str,l_value_str);
  }

  //----------------------------------------------------------------------------
  void generic_dom_utilities<XMLNode>::extract_core_element_attributes(const XMLNode & p_node,
                                                                       osm_object::t_osm_id & p_id,
                                                                       osm_core_element::t_osm_version & p_version,
                                                                       std::string & p_timestamp,
                                                                       std::string & p_user,
                                                                       osm_object::t_osm_id & p_uid,
                                                                       osm_core_element::t_osm_id & p_changeset,
                                                                       bool & p_visible,
                                                                       bool p_search_visible)
  {
    int l_nb_attribute = p_node.nAttribute();
    assert(l_nb_attribute >= 6);
    // Get node id
    XMLCSTR l_id_str = p_node.getAttribute("id");
    assert(l_id_str);
    p_id = strtoull(l_id_str,NULL,10); 
    // Get version
    XMLCSTR l_version_str = p_node.getAttribute("version");
    assert(l_version_str);
    p_version = (osm_core_element::t_osm_version)strtoul(l_version_str,NULL,10);
    // Timestamp
    XMLCSTR l_timestamp_str = p_node.getAttribute("timestamp");
    assert(l_version_str);
    p_timestamp = l_timestamp_str;
    // User
    XMLCSTR l_user_str = p_node.getAttribute("user");
    assert(l_user_str);
    p_user = l_user_str;
    // Get node uid
    XMLCSTR l_uid_str = p_node.getAttribute("uid");
    assert(l_uid_str);
    p_uid = strtoull(l_uid_str,NULL,10); 
    // Get node changeset
    XMLCSTR l_changeset_str = p_node.getAttribute("changeset");
    assert(l_changeset_str);
    p_changeset = strtoull(l_changeset_str,NULL,10); 
    if(p_search_visible)
      {
	p_visible = strcmp(p_node.getAttribute("visible"),"false");
      }
    else
      {
	p_visible = true;
      }
  }

  //----------------------------------------------------------------------------
  void generic_dom_utilities<XMLNode>::extract_node_ref_info(const XMLNode & p_node, osm_way & p_way)
  {
    assert(!strcmp(p_node.getName(),"nd"));
    int l_nb_attribute = p_node.nAttribute();
    assert(l_nb_attribute >= 1);
    XMLCSTR l_ref_str = p_node.getAttribute("ref");
    assert(l_ref_str);
    osm_object::t_osm_id l_node_ref = strtoull(l_ref_str,NULL,10);
    p_way.add_node(l_node_ref);
  }

  //----------------------------------------------------------------------------
  void generic_dom_utilities<XMLNode>::extract_member_info(const XMLNode & p_node, osm_relation & p_relation)
  {
    assert(!strcmp(p_node.getName(),"member"));
    int l_nb_attribute = p_node.nAttribute();
    assert(l_nb_attribute >= 3);

    XMLCSTR l_type_str = p_node.getAttribute("type");
    assert(l_type_str);
    osm_core_element::t_osm_type l_type = osm_core_element::get_osm_type(l_type_str);

    XMLCSTR l_ref_str = p_node.getAttribute("ref");
    assert(l_ref_str);
    osm_object::t_osm_id l_node_ref = strtoull(l_ref_str,NULL,10);

    XMLCSTR l_role_str = p_node.getAttribute("role");
    assert(l_role_str);

    p_relation.add_member(l_type,l_node_ref,std::string(l_role_str));
  }
  
}

#endif // _DOM_UTILITIES_H_
//EOF
