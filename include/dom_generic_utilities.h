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
#ifndef _DOM_UTILITIES_H_
#define _DOM_UTILITIES_H_

#include "osm_node.h"
#include "osm_way.h"
#include "osm_relation.h"
#include "osm_changeset.h"
#include "xmlParser.h"
#include "quicky_exception.h"
#include <sstream>
#include <cstdlib>
#include <cstring>

namespace osm_diff_watcher
{
  template <class T>
    class generic_dom_utilities
    {
    public:
      template <class OSM_OBJ>
        static OSM_OBJ * extract_info(const T & p_node,
                                      bool p_search_visible=false,
                                      bool p_delete_change=false);  

      static void extract_tag_info(const T & p_node,
                                   osm_api_data_types::osm_object & p_object);
      static void extract_node_ref_info(const T & p_node,
                                        osm_api_data_types::osm_way & p_way);
      static void extract_member_info(const T & p_node,
                                      osm_api_data_types::osm_relation & p_relation);
      static void extract_core_element_attributes(const T & p_node,
                                                  osm_api_data_types::osm_object::t_osm_id & p_id,
                                                  osm_api_data_types::osm_core_element::t_osm_version & p_version,
                                                  std::string & p_timestamp,
                                                  std::string & p_user,
                                                  osm_api_data_types::osm_object::t_osm_id & p_uid,
                                                  osm_api_data_types::osm_core_element::t_osm_id & p_changeset,
                                                  bool & p_visible,
                                                  bool p_search_visible);
      static void extract_attributes(const T & p_node,
				     osm_api_data_types::osm_object::t_osm_id & p_id,
				     std::string & p_user,
				     osm_api_data_types::osm_object::t_osm_id & p_uid);
      template <class ATTR_TYPE>
	static ATTR_TYPE extract_attribute(const T & p_node,
                                           const char * p_attribute_name);
    private:
    };

  template <>
    class generic_dom_utilities<XMLNode>
    {
    public:
      template <class OSM_OBJ>
        inline static OSM_OBJ * extract_info(const XMLNode & p_node,bool p_search_visible=false,bool p_delete_change=false);  

      inline static void extract_tag_info(const XMLNode & p_node, osm_api_data_types::osm_object & p_object);
      inline static void extract_node_ref_info(const XMLNode & p_node, osm_api_data_types::osm_way & p_way);
      inline static void extract_member_info(const XMLNode & p_node, osm_api_data_types::osm_relation & p_relation);
      inline static void extract_core_element_attributes(const XMLNode & p_node,
                                                         osm_api_data_types::osm_object::t_osm_id & p_id,
                                                         osm_api_data_types::osm_core_element::t_osm_version & p_version,
                                                         std::string & p_timestamp,
                                                         std::string & p_user,
                                                         osm_api_data_types::osm_object::t_osm_id & p_uid,
                                                         osm_api_data_types::osm_core_element::t_osm_id & p_changeset,
                                                         bool & p_visible,
                                                         bool p_search_visible);
      inline static void extract_attributes(const XMLNode & p_node,
					    osm_api_data_types::osm_object::t_osm_id & p_id,
					    std::string & p_user,
					    osm_api_data_types::osm_object::t_osm_id & p_uid);
      template <class ATTR_TYPE>
	inline static ATTR_TYPE extract_attribute(const XMLNode & p_node,const char * p_attribute_name);
    private:
    };

  //----------------------------------------------------------------------------
  template <class OSM_OBJ>
    OSM_OBJ * generic_dom_utilities<XMLNode>::extract_info(const XMLNode & p_node,bool p_search_visible,bool p_delete_change)
    {
      // Dummy implementation should not be called
      std::stringstream l_stream;
      l_stream << "No implementation provided for type \"" << OSM_OBJ::get_type_str() << "\"" ;
      throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
      return NULL;
    }

  //----------------------------------------------------------------------------
  template <class ATTR_TYPE>
    ATTR_TYPE extract_attribute(const XMLNode & p_node,const char * p_attribute_name)
    {
      // Dummy implementation should not be called
      std::stringstream l_stream;
      l_stream << "No implementation provided for " << __FUNCTION__ << " of attribute \"" << p_attribute_name << "\" of node \"" << p_node.getName() << "\"" ;
      throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
      return NULL;
    }

  //----------------------------------------------------------------------------
  template <>
    inline const char * generic_dom_utilities<XMLNode>::extract_attribute(const XMLNode & p_node,
                                                                          const char * p_attribute_name)
    {
      XMLCSTR l_str = p_node.getAttribute(p_attribute_name);
      if(NULL == l_str)
        {
          std::stringstream l_stream;
          l_stream << "Mandatory attribute \"" << p_attribute_name << "\" is missing in node \"" << p_node.getName() << "\"";
          throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
        }
      return l_str;
    }

  //----------------------------------------------------------------------------
  template <>
    inline osm_api_data_types::osm_object::t_osm_id generic_dom_utilities<XMLNode>::extract_attribute(const XMLNode & p_node,
                                                                                                      const char * p_attribute_name)
    {
      XMLCSTR l_str = p_node.getAttribute(p_attribute_name);
      if(NULL == l_str)
        {
          std::stringstream l_stream;
          l_stream << "Mandatory attribute \"" << p_attribute_name << "\" is missing in node \"" << p_node.getName() << "\"";
          throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
        }
      return strtoull(l_str,NULL,10); 
    }

  //----------------------------------------------------------------------------
  template <>
    inline osm_api_data_types::osm_core_element::t_osm_version generic_dom_utilities<XMLNode>::extract_attribute(const XMLNode & p_node,
                                                                                                                 const char * p_attribute_name)
    {
      XMLCSTR l_str = p_node.getAttribute(p_attribute_name);
      if(NULL == l_str)
        {
          std::stringstream l_stream;
          l_stream << "Mandatory attribute \"" << p_attribute_name << "\" is missing in node \"" << p_node.getName() << "\"";
          throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
        }
      return strtoul(l_str,NULL,10); 
    }

  //----------------------------------------------------------------------------
  template <>
    inline float generic_dom_utilities<XMLNode>::extract_attribute(const XMLNode & p_node,
								   const char * p_attribute_name)
    {
      XMLCSTR l_str = p_node.getAttribute(p_attribute_name);
      return (l_str ? atof(l_str):0.0); 
    }

  //----------------------------------------------------------------------------
  template <>
    inline bool generic_dom_utilities<XMLNode>::extract_attribute(const XMLNode & p_node,
                                                                  const char * p_attribute_name)
    {
      XMLCSTR l_str = p_node.getAttribute(p_attribute_name);
      if(NULL == l_str)
        {
          std::stringstream l_stream;
          l_stream << "Mandatory attribute \"" << p_attribute_name << "\" is missing in node \"" << p_node.getName() << "\"";
          throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
        }
      return !strcmp(l_str,"true"); 
    }

  //----------------------------------------------------------------------------
  template <>
    inline osm_api_data_types::osm_core_element::t_osm_type generic_dom_utilities<XMLNode>::extract_attribute(const XMLNode & p_node,
                                                                                                              const char * p_attribute_name)
    {
      XMLCSTR l_type_str = p_node.getAttribute(p_attribute_name);
      if(NULL == l_type_str)
        {
          std::stringstream l_stream;
          l_stream << "Mandatory attribute \"" << p_attribute_name << "\" is missing in node \"" << p_node.getName() << "\"";
          throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
        }
      return osm_api_data_types::osm_core_element::get_osm_type(l_type_str);
    }

  //----------------------------------------------------------------------------
  template <>
    inline osm_api_data_types::osm_node * generic_dom_utilities<XMLNode>::extract_info<osm_api_data_types::osm_node>(const XMLNode & p_node,bool p_search_visible,bool p_delete_change)
    {
      osm_api_data_types::osm_object::t_osm_id l_id;
      osm_api_data_types::osm_core_element::t_osm_version l_version;
      std::string l_timestamp;
      std::string l_user;
      osm_api_data_types::osm_object::t_osm_id l_uid;
      osm_api_data_types::osm_core_element::t_osm_id l_changeset;
      bool l_visible;

      extract_core_element_attributes(p_node,l_id,l_version,l_timestamp,l_user,l_uid,l_changeset,l_visible,p_search_visible);

      float l_lat = (l_visible && !p_delete_change ? extract_attribute<float>(p_node,"lat") : 0.0);
      float l_lon = (l_visible && !p_delete_change ? extract_attribute<float>(p_node,"lon") : 0.0);

      osm_api_data_types::osm_node * l_osm_node = new osm_api_data_types::osm_node(l_id,l_lat,l_lon,l_timestamp,l_version,l_changeset,l_uid,l_user);
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
    inline osm_api_data_types::osm_way * generic_dom_utilities<XMLNode>::extract_info(const XMLNode & p_node,bool p_search_visible,bool p_delete_change)
    {
      osm_api_data_types::osm_object::t_osm_id l_id;
      osm_api_data_types::osm_core_element::t_osm_version l_version;
      std::string l_timestamp;
      std::string l_user;
      osm_api_data_types::osm_object::t_osm_id l_uid;
      osm_api_data_types::osm_core_element::t_osm_id l_changeset;
      bool l_visible;

      extract_core_element_attributes(p_node,l_id,l_version,l_timestamp,l_user,l_uid,l_changeset,l_visible,p_search_visible);

      osm_api_data_types::osm_way * l_osm_way = new osm_api_data_types::osm_way(l_id,l_timestamp,l_version,l_changeset,l_uid,l_user);
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
    inline osm_api_data_types::osm_relation * generic_dom_utilities<XMLNode>::extract_info(const XMLNode & p_node,bool p_search_visible,bool p_delete_change)
    {
      osm_api_data_types::osm_object::t_osm_id l_id;
      osm_api_data_types::osm_core_element::t_osm_version l_version;
      std::string l_timestamp;
      std::string l_user;
      osm_api_data_types::osm_object::t_osm_id l_uid;
      osm_api_data_types::osm_core_element::t_osm_id l_changeset;
      bool l_visible;

      extract_core_element_attributes(p_node,l_id,l_version,l_timestamp,l_user,l_uid,l_changeset,l_visible,p_search_visible);

      osm_api_data_types::osm_relation * l_osm_relation = new osm_api_data_types::osm_relation(l_id,l_timestamp,l_version,l_changeset,l_uid,l_user);
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
  template <>
    inline osm_api_data_types::osm_changeset * generic_dom_utilities<XMLNode>::extract_info(const XMLNode & p_node,bool p_search_visible,bool p_delete_change)
    {
      osm_api_data_types::osm_object::t_osm_id l_id;
      std::string l_user;
      osm_api_data_types::osm_object::t_osm_id l_uid;

      extract_attributes(p_node,l_id,l_user,l_uid);

      std::string l_created_at(extract_attribute<const char *>(p_node,"created_at"));
      std::string l_closed_at(extract_attribute<const char *>(p_node,"closed_at"));
      bool l_open = extract_attribute<bool>(p_node,"open");
      float l_min_lat = extract_attribute<float>(p_node,"min_lat");
      float l_min_lon = extract_attribute<float>(p_node,"min_lon");
      float l_max_lat = extract_attribute<float>(p_node,"max_lat");
      float l_max_lon = extract_attribute<float>(p_node,"max_lon");

      osm_api_data_types::osm_changeset * l_changeset = new osm_api_data_types::osm_changeset(l_id,l_uid,l_user,l_created_at,l_closed_at,l_open,osm_api_data_types::osm_bounding_box(l_min_lat,l_min_lon,l_max_lat,l_max_lon));

      int l_nb_child_object = p_node.nChildNode();
      for(int l_index = 0 ; l_index < l_nb_child_object ; ++l_index)
	{
	  extract_tag_info(p_node.getChildNode(l_index),*l_changeset);
	}
      return l_changeset;
    }
  //----------------------------------------------------------------------------
  void generic_dom_utilities<XMLNode>::extract_tag_info(const XMLNode & p_node, osm_api_data_types::osm_object & p_object)
  {
    if(strcmp(p_node.getName(),"tag"))
      {
        std::stringstream l_stream;
        l_stream << "Name of node should be \"tag\" instead of \"" << p_node.getName() << "\"" ;
        throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
      }

    int l_nb_attribute = p_node.nAttribute();

    if(2 > l_nb_attribute)
      {
        throw quicky_exception::quicky_logic_exception("Child number should be 2 for node \"tag\"",__LINE__,__FILE__);
      }

    XMLCSTR l_key_str = p_node.getAttribute("k");
    if(NULL == l_key_str)
      {
        std::stringstream l_stream;
        l_stream << "Mandatory attribute \"k\" is missing in node \"" << p_node.getName() << "\"";
        throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
      }
    XMLCSTR l_value_str = p_node.getAttribute("v",0);
    if(NULL == l_value_str)
      {
        std::stringstream l_stream;
        l_stream << "Mandatory attribute \"v\" is missing in node \"" << p_node.getName() << "\"";
        throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
      }
    p_object.add_tag(l_key_str,l_value_str);
  }

  //----------------------------------------------------------------------------
  void generic_dom_utilities<XMLNode>::extract_core_element_attributes(const XMLNode & p_node,
                                                                       osm_api_data_types::osm_object::t_osm_id & p_id,
                                                                       osm_api_data_types::osm_core_element::t_osm_version & p_version,
                                                                       std::string & p_timestamp,
                                                                       std::string & p_user,
                                                                       osm_api_data_types::osm_object::t_osm_id & p_uid,
                                                                       osm_api_data_types::osm_core_element::t_osm_id & p_changeset,
                                                                       bool & p_visible,
                                                                       bool p_search_visible)
  {
    int l_nb_attribute = p_node.nAttribute();
    if(6 > l_nb_attribute)
      {
        std::stringstream l_stream;
        l_stream << "Child number should be 6 at least for node \"" << p_node.getName() << "\"" ;
        throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
      }


    extract_attributes(p_node,p_id,p_user,p_uid);

    // Version
    p_version = extract_attribute<osm_api_data_types::osm_core_element::t_osm_version>(p_node,"version");

    // Timestamp
    p_timestamp = extract_attribute<const char *>(p_node,"timestamp");

    // Get node changeset
    p_changeset = extract_attribute<osm_api_data_types::osm_core_element::t_osm_id>(p_node,"changeset");

    if(p_search_visible && p_node.isAttributeSet("visible"))
      {
        p_visible = extract_attribute<bool>(p_node,"visible");
      }
    else
      {
	p_visible = true;
      }
  }

  //----------------------------------------------------------------------------
  void generic_dom_utilities<XMLNode>::extract_attributes(const XMLNode & p_node,
							  osm_api_data_types::osm_object::t_osm_id & p_id,
							  std::string & p_user,
							  osm_api_data_types::osm_object::t_osm_id & p_uid)
  {
    int l_nb_attribute = p_node.nAttribute();
    if(3 > l_nb_attribute)
      {
        std::stringstream l_stream;
        l_stream << "Child number should be 3 at least for node \"" << p_node.getName() << "\"" ;
        throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
      }

    p_id = extract_attribute<osm_api_data_types::osm_object::t_osm_id>(p_node,"id");
    p_user = extract_attribute<const char *>(p_node,"user");
    p_uid = extract_attribute<osm_api_data_types::osm_object::t_osm_id>(p_node,"uid");
  }

  //----------------------------------------------------------------------------
  void generic_dom_utilities<XMLNode>::extract_node_ref_info(const XMLNode & p_node, osm_api_data_types::osm_way & p_way)
  {
    if(strcmp("nd",p_node.getName()))
      {
        std::stringstream l_stream;
        l_stream << "Root of XML tree should be \"nd\" instead of \"" << p_node.getName() << "\"" ;
        throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
      }
    int l_nb_attribute = p_node.nAttribute();
    if(1 > l_nb_attribute)
      {
        std::stringstream l_stream;
        l_stream << "Child number should be 1 at least for node \"" << p_node.getName() << "\"" ;
        throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
      }

    osm_api_data_types::osm_object::t_osm_id l_node_ref = extract_attribute<osm_api_data_types::osm_object::t_osm_id>(p_node,"ref");
    p_way.add_node(l_node_ref);
  }

  //----------------------------------------------------------------------------
  void generic_dom_utilities<XMLNode>::extract_member_info(const XMLNode & p_node, osm_api_data_types::osm_relation & p_relation)
  {
    if(strcmp("member",p_node.getName()))
      {
        std::stringstream l_stream;
        l_stream << "Root of XML tree should be \"member\" instead of \"" << p_node.getName() << "\"" ;
        throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
      }
    int l_nb_attribute = p_node.nAttribute();
    if(3 > l_nb_attribute)
      {
        std::stringstream l_stream;
        l_stream << "Child number should be 3 at least for node \"" << p_node.getName() << "\"" ;
        throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
      }

    osm_api_data_types::osm_core_element::t_osm_type l_type = extract_attribute<osm_api_data_types::osm_core_element::t_osm_type>(p_node,"type");
    osm_api_data_types::osm_object::t_osm_id l_node_ref = extract_attribute<osm_api_data_types::osm_object::t_osm_id>(p_node,"ref");
    const std::string l_role = extract_attribute<const char *>(p_node,"role");

    p_relation.add_member(l_type,l_node_ref,l_role);
  }
  
}

#endif // _DOM_UTILITIES_H_
//EOF
