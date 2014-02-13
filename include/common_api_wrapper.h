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
#ifndef _COMMON_API_WRAPPER_H_
#define _COMMON_API_WRAPPER_H_

#include "common_api_if.h"
#include "osm_ressources.h"
#include "osm_api.h"
#include "osm_cache.h"
#include "soda_Ui_if.h"

namespace osm_diff_watcher
{
  class common_api_wrapper
  {
  public:
    static common_api_wrapper & instance(osm_ressources & p_ressources,soda_Ui_if & p_Ui);
    static void remove_instance(void);
    static void register_function(uintptr_t *,uint32_t);

  private:
    static const char * get_api_version(void);
    inline static void get_user_subscription_date(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                  const std::string & p_name,
                                                  std::string & p_date,
                                                  void * p_user_data=NULL);

    inline static const osm_api_data_types::osm_node * get_node(const osm_api_data_types::osm_object::t_osm_id & p_id,
								void * p_user_data=NULL);

    inline static const osm_api_data_types::osm_node * get_node_version(const osm_api_data_types::osm_object::t_osm_id & p_id,
									const osm_api_data_types::osm_core_element::t_osm_version & p_version,
									void * p_user_data=NULL);

    inline static const std::vector<osm_api_data_types::osm_node*> * const get_node_history(const osm_api_data_types::osm_object::t_osm_id & p_id,
											    void * p_user_data = NULL);

    inline static const std::vector<osm_api_data_types::osm_way*> * const get_node_ways(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                                                        void * p_user_data = NULL);

    inline static const std::vector<osm_api_data_types::osm_relation*> * const get_node_relations(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                                                                  void * p_user_data = NULL);

    inline static const std::vector<osm_api_data_types::osm_node*> * const get_nodes(const std::vector<osm_api_data_types::osm_object::t_osm_id> & p_ids,
										     void * p_user_data=NULL);

    inline static const osm_api_data_types::osm_way * get_way(const osm_api_data_types::osm_object::t_osm_id & p_id,
							      void * p_user_data=NULL);

    inline static const osm_api_data_types::osm_way * get_way_version(const osm_api_data_types::osm_object::t_osm_id & p_id,
								      const osm_api_data_types::osm_core_element::t_osm_version & p_version,
								      void * p_user_data=NULL);

    inline static const std::vector<osm_api_data_types::osm_way*> * const get_way_history(const osm_api_data_types::osm_object::t_osm_id & p_id,
											  void * p_user_data = NULL);

    inline static const std::vector<osm_api_data_types::osm_relation*> * const get_way_relations(const osm_api_data_types::osm_object::t_osm_id & p_id,
												 void * p_user_data = NULL);

    inline static const osm_api_data_types::osm_way * const get_way_full(const osm_api_data_types::osm_object::t_osm_id & p_id,
									 std::vector<osm_api_data_types::osm_node*> & p_nodes,
									 void * p_user_data=NULL);

    inline static const std::vector<osm_api_data_types::osm_way*> * const get_ways(const std::vector<osm_api_data_types::osm_object::t_osm_id> & p_ids,
										   void * p_user_data=NULL);

    inline static const osm_api_data_types::osm_relation * get_relation(const osm_api_data_types::osm_object::t_osm_id & p_id,
									void * p_user_data=NULL);

    inline static const osm_api_data_types::osm_relation * get_relation_version(const osm_api_data_types::osm_object::t_osm_id & p_id,
										const osm_api_data_types::osm_core_element::t_osm_version & p_version,
										void * p_user_data=NULL);

    inline static const std::vector<osm_api_data_types::osm_relation*> * const get_relation_history(const osm_api_data_types::osm_object::t_osm_id & p_id,
												    void * p_user_data = NULL);

    inline static const std::vector<osm_api_data_types::osm_relation*> * const get_relation_relations(const osm_api_data_types::osm_object::t_osm_id & p_id,
												      void * p_user_data = NULL);

    inline static const osm_api_data_types::osm_relation * const get_relation_full(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                                                   std::vector<osm_api_data_types::osm_node*> & p_nodes,
                                                                                   std::vector<osm_api_data_types::osm_way*> & p_ways,
                                                                                   void * p_user_data=NULL);

    inline static const std::vector<osm_api_data_types::osm_relation*> * const get_relations(const std::vector<osm_api_data_types::osm_object::t_osm_id> & p_ids,
											     void * p_user_data=NULL);
    inline static const osm_api_data_types::osm_changeset * get_changeset(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                                          void * p_user_data=NULL);
    inline static const std::vector<osm_api_data_types::osm_change*> * const get_changeset_content(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                                                                   void * p_user_data = NULL);

    inline static const std::vector<osm_api_data_types::osm_changeset* > * const get_changesets(const osm_api_data_types::osm_bounding_box & p_bounding_box,
                                                                                                const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                                                                const std::string & p_user_name,
                                                                                                const std::string & p_time1,
                                                                                                const std::string & p_time2,
                                                                                                bool p_open,
                                                                                                bool p_close,                                                               
                                                                                                void * p_user_data=NULL);

    inline static void get_map(const osm_api_data_types::osm_bounding_box & p_bounding_box,
                               std::vector<osm_api_data_types::osm_node*> & p_nodes,
                               std::vector<osm_api_data_types::osm_way*> & p_ways,
                               std::vector<osm_api_data_types::osm_relation*> & p_relations,
                               void *p_user_data=NULL);
    
    inline static void cache_node(const osm_api_data_types::osm_node & p_node);
    inline static void cache_way(const osm_api_data_types::osm_way & p_way);
    inline static void cache_relation(const osm_api_data_types::osm_relation & p_relation);
    inline static void cache_user(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                  const std::string & p_user_name,
				  const osm_api_data_types::osm_object::t_osm_id & p_latest_changeset,
                                  const std::string & p_date);
    inline static const std::vector<osm_api_data_types::osm_change*> * const get_osm_change_file_content(const std::string & p_file_name);
    inline static  void get_osm_file_content(const std::string & p_file_name,
					     std::vector<osm_api_data_types::osm_node*> & p_nodes,
					     std::vector<osm_api_data_types::osm_way*> & p_ways,
					     std::vector<osm_api_data_types::osm_relation*> & p_relations);

    inline static void get_user_browse_url(std::string & p_result,
                                           const osm_api_data_types::osm_object::t_osm_id & p_id,
                                           const std::string & p_user_name);
    inline static void get_object_browse_url(std::string & p_result,
                                             const std::string & p_type,
                                             const osm_api_data_types::osm_object::t_osm_id & p_id);
    inline static void get_api_object_url(std::string & p_result,
                                          const std::string & p_type,
                                          const osm_api_data_types::osm_object::t_osm_id & p_id,
                                          const osm_api_data_types::osm_core_element::t_osm_version & p_version);

    inline static void ui_register_module(const osm_diff_analyzer_if::analyzer_base & p_module,
					  const std::string & p_name);

    inline static void ui_append_log_text(const osm_diff_analyzer_if::analyzer_base & p_module,
		                      const std::string & p_text);

    inline static void ui_declare_html_report(const osm_diff_analyzer_if::analyzer_base & p_module,
					      const std::string & p_name);

    common_api_wrapper(osm_ressources & p_ressources,soda_Ui_if & p_Ui);
    static common_api_wrapper * m_instance;

    soda_Ui_if & m_Ui;
    osm_cache m_cache;
    osm_ressources & m_ressources;
    osm_api m_api;
  };
  
  //----------------------------------------------------------------------------
  void common_api_wrapper::get_user_subscription_date(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                      const std::string & p_name,
                                                      std::string & p_date,
                                                      void * p_user_data)
  {
    std::string l_name;
    m_instance->m_cache.load_user(p_id,l_name,p_date);
    if(p_date == "")
      {
	m_instance->m_ressources.get_user_subscription_date(p_name,p_date);
      }
  }
  //----------------------------------------------------------------------------
  const osm_api_data_types::osm_node * common_api_wrapper::get_node(const osm_api_data_types::osm_object::t_osm_id & p_id,
								    void * p_user_data)
    {
      return m_instance->m_api.get<osm_api_data_types::osm_node>(p_id);
    }
  //----------------------------------------------------------------------------
  const osm_api_data_types::osm_node * common_api_wrapper::get_node_version(const osm_api_data_types::osm_object::t_osm_id & p_id,
									    const osm_api_data_types::osm_core_element::t_osm_version & p_version,
									    void * p_user_data)
    {
      const osm_api_data_types::osm_node * l_result = m_instance->m_cache.load_node(p_id,p_version);
      return (l_result != NULL ? l_result : m_instance->m_api.get<osm_api_data_types::osm_node>(p_id,p_version));
    }

  //----------------------------------------------------------------------------
  const std::vector<osm_api_data_types::osm_node*> * const common_api_wrapper::get_node_history(const osm_api_data_types::osm_object::t_osm_id & p_id,
												void * p_user_data)
    {
      return m_instance->m_api.get_history<osm_api_data_types::osm_node>(p_id);
    }
  //----------------------------------------------------------------------------
  const std::vector<osm_api_data_types::osm_way*> * const common_api_wrapper::get_node_ways(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                                                            void * p_user_data)
    {
      return m_instance->m_api.get<osm_api_data_types::osm_node,osm_api_data_types::osm_way>(p_id);
    }

  //----------------------------------------------------------------------------
  const std::vector<osm_api_data_types::osm_relation*> * const common_api_wrapper::get_node_relations(const osm_api_data_types::osm_object::t_osm_id & p_id,
												      void * p_user_data)
    {
      return m_instance->m_api.get<osm_api_data_types::osm_node,osm_api_data_types::osm_relation>(p_id);
    }

  //----------------------------------------------------------------------------
  const std::vector<osm_api_data_types::osm_node*> * const common_api_wrapper::get_nodes(const std::vector<osm_api_data_types::osm_object::t_osm_id> & p_ids,
											 void * p_user_data)
    {
      return m_instance->m_api.get<osm_api_data_types::osm_node>(p_ids);
    }
  //----------------------------------------------------------------------------
  const osm_api_data_types::osm_way * common_api_wrapper::get_way(const osm_api_data_types::osm_object::t_osm_id & p_id,
								  void * p_user_data)
    {
      return m_instance->m_api.get<osm_api_data_types::osm_way>(p_id);
    }
  //----------------------------------------------------------------------------
  const osm_api_data_types::osm_way * common_api_wrapper::get_way_version(const osm_api_data_types::osm_object::t_osm_id & p_id,
									  const osm_api_data_types::osm_core_element::t_osm_version & p_version,
									  void * p_user_data)
    {
      const osm_api_data_types::osm_way * l_result = m_instance->m_cache.load_way(p_id,p_version);
      return (l_result != NULL ? l_result : m_instance->m_api.get<osm_api_data_types::osm_way>(p_id,p_version));
    }
  //----------------------------------------------------------------------------
  const std::vector<osm_api_data_types::osm_way*> * const common_api_wrapper::get_way_history(const osm_api_data_types::osm_object::t_osm_id & p_id,
											      void * p_user_data)
    {
      return m_instance->m_api.get_history<osm_api_data_types::osm_way>(p_id);
    }
  //----------------------------------------------------------------------------
  const std::vector<osm_api_data_types::osm_relation*> * const common_api_wrapper::get_way_relations(const osm_api_data_types::osm_object::t_osm_id & p_id,
												     void * p_user_data)
    {
      return m_instance->m_api.get<osm_api_data_types::osm_way,osm_api_data_types::osm_relation>(p_id);
    }

  //----------------------------------------------------------------------------
  const osm_api_data_types::osm_way * const common_api_wrapper::get_way_full(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                                             std::vector<osm_api_data_types::osm_node*> & p_nodes,
                                                                             void * p_user_data)
    {
      return m_instance->m_api.get_full_way(p_id,p_nodes);
    }
  //----------------------------------------------------------------------------
  const std::vector<osm_api_data_types::osm_way*> * const common_api_wrapper::get_ways(const std::vector<osm_api_data_types::osm_object::t_osm_id> & p_ids,
										       void * p_user_data)
    {
      return m_instance->m_api.get<osm_api_data_types::osm_way>(p_ids);
    }
  //----------------------------------------------------------------------------
  const osm_api_data_types::osm_relation * common_api_wrapper::get_relation(const osm_api_data_types::osm_object::t_osm_id & p_id,
									    void * p_user_data)
    {
      return m_instance->m_api.get<osm_api_data_types::osm_relation>(p_id);
    }
  //----------------------------------------------------------------------------
  const osm_api_data_types::osm_relation * common_api_wrapper::get_relation_version(const osm_api_data_types::osm_object::t_osm_id & p_id,
										    const osm_api_data_types::osm_core_element::t_osm_version & p_version,
										    void * p_user_data)
    {
      const osm_api_data_types::osm_relation * l_result = m_instance->m_cache.load_relation(p_id,p_version);
      return (l_result != NULL ? l_result : m_instance->m_api.get<osm_api_data_types::osm_relation>(p_id,p_version));
    }
  //----------------------------------------------------------------------------
  const std::vector<osm_api_data_types::osm_relation*> * const common_api_wrapper::get_relation_history(const osm_api_data_types::osm_object::t_osm_id & p_id,
													void * p_user_data)
    {
      return m_instance->m_api.get_history<osm_api_data_types::osm_relation>(p_id);
    }
  //----------------------------------------------------------------------------
  const std::vector<osm_api_data_types::osm_relation*> * const common_api_wrapper::get_relation_relations(const osm_api_data_types::osm_object::t_osm_id & p_id,
													  void * p_user_data)
    {
      return m_instance->m_api.get<osm_api_data_types::osm_relation,osm_api_data_types::osm_relation>(p_id);
    }

  //----------------------------------------------------------------------------
  const osm_api_data_types::osm_relation * const common_api_wrapper::get_relation_full(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                                                       std::vector<osm_api_data_types::osm_node*> & p_nodes,
                                                                                       std::vector<osm_api_data_types::osm_way*> & p_ways,
                                                                                       void * p_user_data)
    {
      return m_instance->m_api.get_full_relation(p_id,p_nodes,p_ways);
    }
  //----------------------------------------------------------------------------
  const std::vector<osm_api_data_types::osm_relation*> * const common_api_wrapper::get_relations(const std::vector<osm_api_data_types::osm_object::t_osm_id> & p_ids,
												 void * p_user_data)
    {
      return m_instance->m_api.get<osm_api_data_types::osm_relation>(p_ids);
    }

  //----------------------------------------------------------------------------
  const osm_api_data_types::osm_changeset * common_api_wrapper::get_changeset(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                                              void * p_user_data)
    {
      return m_instance->m_api.get<osm_api_data_types::osm_changeset>(p_id);
    }
  //----------------------------------------------------------------------------
  const std::vector<osm_api_data_types::osm_change*> * const common_api_wrapper::get_changeset_content(const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                                                                       void * p_user_data)
    {
      return m_instance->m_api.get_changeset_content(p_id);
    }

  //----------------------------------------------------------------------------
  const std::vector<osm_api_data_types::osm_changeset* > * const common_api_wrapper::get_changesets(const osm_api_data_types::osm_bounding_box & p_bounding_box,
                                                                                                    const osm_api_data_types::osm_object::t_osm_id & p_id,
                                                                                                    const std::string & p_user_name,
                                                                                                    const std::string & p_time1,
                                                                                                    const std::string & p_time2,
                                                                                                    bool p_open,
                                                                                                    bool p_close,
                                                                                                    void * p_user_data)
    {
      return m_instance->m_api.get_changesets(p_bounding_box,p_id,p_user_name,p_time1,p_time2,p_open,p_close);
    }

  //----------------------------------------------------------------------------
  void common_api_wrapper::get_map(const osm_api_data_types::osm_bounding_box & p_bounding_box,
				   std::vector<osm_api_data_types::osm_node*> & p_nodes,
				   std::vector<osm_api_data_types::osm_way*> & p_ways,
				   std::vector<osm_api_data_types::osm_relation*> & p_relations,
				   void *p_user_data)
  {
    return m_instance->m_api.get_map(p_bounding_box,p_nodes,p_ways,p_relations);
  }

  //----------------------------------------------------------------------------
  void common_api_wrapper::cache_node(const osm_api_data_types::osm_node & p_node)
  {
    m_instance->m_cache.store(p_node);
  }

  //----------------------------------------------------------------------------
  void common_api_wrapper::cache_way(const osm_api_data_types::osm_way & p_way)
  {
    m_instance->m_cache.store(p_way);
  }

  //----------------------------------------------------------------------------
  void common_api_wrapper::cache_relation(const osm_api_data_types::osm_relation & p_relation)
  {
    m_instance->m_cache.store(p_relation);
  }
  //----------------------------------------------------------------------------
  void common_api_wrapper::cache_user(const osm_api_data_types::osm_object::t_osm_id & p_id,
				      const std::string & p_user_name,
				      const osm_api_data_types::osm_object::t_osm_id & p_latest_changeset,
				      const std::string & p_date)
  {
    m_instance->m_cache.store(p_id,p_user_name,p_latest_changeset,p_date);
  }
  //----------------------------------------------------------------------------
  const std::vector<osm_api_data_types::osm_change*> * const common_api_wrapper::get_osm_change_file_content(const std::string & p_file_name)
    {
      return osm_ressources::get_osm_change_file_content(p_file_name);
    }

  //----------------------------------------------------------------------------
  void common_api_wrapper::get_osm_file_content(const std::string & p_file_name,
						std::vector<osm_api_data_types::osm_node*> & p_nodes,
						std::vector<osm_api_data_types::osm_way*> & p_ways,
						std::vector<osm_api_data_types::osm_relation*> & p_relations)
  {
    osm_ressources::get_osm_file_content(p_file_name,p_nodes,p_ways,p_relations);
  }
 
  //----------------------------------------------------------------------------
  void common_api_wrapper::get_user_browse_url(std::string & p_result,
                                               const osm_api_data_types::osm_object::t_osm_id & p_id,
                                               const std::string & p_user_name)
  {
    m_instance->m_ressources.get_user_browse_url(p_result,p_user_name);
  }

  //----------------------------------------------------------------------------
  void common_api_wrapper::get_object_browse_url(std::string & p_result,
                                                 const std::string & p_type,
                                                 const osm_api_data_types::osm_object::t_osm_id & p_id)
  {
    m_instance->m_ressources.get_object_browse_url(p_result,p_type,p_id);
  }

  //----------------------------------------------------------------------------
  void common_api_wrapper::get_api_object_url(std::string & p_result,
                                              const std::string & p_type,
                                              const osm_api_data_types::osm_object::t_osm_id & p_id,
                                              const osm_api_data_types::osm_core_element::t_osm_version & p_version)
  {
    m_instance->m_ressources.get_api_object_url(p_result,p_type,p_id,p_version);
  }

  //----------------------------------------------------------------------------
  void common_api_wrapper::ui_register_module(const osm_diff_analyzer_if::analyzer_base & p_module,
					      const std::string & p_name)
  {
    m_instance->m_Ui.instantiate_module(p_name,p_module);
  }

  //----------------------------------------------------------------------------
  void common_api_wrapper::ui_append_log_text(const osm_diff_analyzer_if::analyzer_base & p_module,
                                          const std::string & p_text)
  {
    m_instance->m_Ui.append_log_text(p_module,p_text);
  }
  
  //----------------------------------------------------------------------------
  void common_api_wrapper::ui_declare_html_report(const osm_diff_analyzer_if::analyzer_base & p_module,
						  const std::string & p_name)
  {
    m_instance->m_Ui.declare_html_report(p_module,p_name);
  }
  
}
#endif // _COMMON_API_WRAPPER_H_
//EOF
