#include "osm_cache.h"
#include <cassert>

namespace osm_diff_watcher
{
  //----------------------------------------------------------------------------
  osm_cache::osm_cache(const std::string & p_name):
    m_db(NULL),
    m_get_node_tags_stmt(NULL),
    m_get_way_tags_stmt(NULL),
    m_get_relation_tags_stmt(NULL),
    m_get_relation_members_stmt(NULL),
    // Simple key tables
    m_tag_name_table("tag_names"),
    m_tag_value_table("tag_values"),
    m_relation_role_table("relation_roles"),
    // Element tables
    m_way_table("ways"),
    m_relation_table("relations"),
    // Tag tables
    m_node_tag_table("node_tags"),
    m_way_tag_table("way_tags"),
    m_relation_tag_table("relation_tags")
  {
    // Opening the database
    int l_status = sqlite3_open(p_name.c_str(), &m_db);
    if(l_status == SQLITE_OK)
      {
	m_tag_name_table.set_db(m_db);
	m_tag_value_table.set_db(m_db);
	m_relation_role_table.set_db(m_db);
	m_user_table.set_db(m_db);
	m_node_table.set_db(m_db);
	m_way_table.set_db(m_db);
	m_relation_table.set_db(m_db);
	m_node_tag_table.set_db(m_db);
	m_way_tag_table.set_db(m_db);
	m_relation_tag_table.set_db(m_db);
	m_way_member_table.set_db(m_db);
	m_relation_member_table.set_db(m_db);


        prepare_get_tags_statement(m_node_tag_table.get_name(),m_get_node_tags_stmt);
        prepare_get_tags_statement(m_way_tag_table.get_name(),m_get_way_tags_stmt);
        prepare_get_tags_statement(m_relation_tag_table.get_name(),m_get_relation_tags_stmt);
       
	// Preparing get_relation_members statements
	//--------------------------------------------
	std::string l_stmt_str = "SELECT Osm_type, Ref, Name, Position, "+ 
	  m_relation_member_table.get_name()+".Id, " + 
	  m_relation_member_table.get_name()+".Version " + 
	  "FROM "+
	  m_relation_member_table.get_name() + "," +
	  m_relation_role_table.get_name() + " "
	  "WHERE "+
	  m_relation_member_table.get_name() + ".Id = $id " +
	  "AND Version = $version " +
	  "AND "+ m_relation_role_table.get_name() + ".Id = Role_id "+
	  "ORDER BY Position ASC"+
	  ";";
	std::cout << l_stmt_str << std::endl ;
	l_status = sqlite3_prepare_v2(m_db,(l_stmt_str
					    ).c_str(),
                                      -1,
                                      &m_get_relation_members_stmt,
                                      NULL);
	if(l_status != SQLITE_OK)
	  {
	    std::cout << "ERROR during preparation of statement to get_relation_members : " << sqlite3_errmsg(m_db) << std::endl ;     
	    exit(-1);
	  }

 
      }
    else
      {
        std::cerr << "Can't open database \"" << p_name << "\" : " << sqlite3_errmsg(m_db) << std::endl ;
      }
    std::cout << "Database \"" << p_name << "\" successfully opened" << std::endl ; 
  }

  //----------------------------------------------------------------------------
  void osm_cache::prepare_get_tags_statement(const std::string & p_table_name,sqlite3_stmt * & p_stmt_ptr)
  {
    // Preparing get_tags statements
    //--------------------------------------------
    int l_status = sqlite3_prepare_v2(m_db,("SELECT "+
                                            m_tag_name_table.get_name()+".Name,"+
                                            m_tag_value_table.get_name()+".Name "+
                                            "FROM " + 
                                            m_tag_name_table.get_name() + "," +
                                            m_tag_value_table.get_name() + "," +
                                            p_table_name + " " +
                                            "WHERE "+
                                            p_table_name + ".Id == $id AND "+
                                            "Version = $version AND "+
                                            m_tag_name_table.get_name() + ".Id == Tag_Name_Id AND "+
                                            m_tag_value_table.get_name() + ".Id == Tag_Value_Id" +
                                            ";"
                                            ).c_str(),
                                      -1,
                                      &p_stmt_ptr,
                                      NULL);
    if(l_status != SQLITE_OK)
      {
        std::cout << "ERROR during preparation of statement to get_tags of object : " << sqlite3_errmsg(m_db) << std::endl ;     
        exit(-1);
      }
  }

  //----------------------------------------------------------------------------
  osm_cache::~osm_cache(void)
  {
    sqlite3_finalize(m_get_relation_members_stmt);
    sqlite3_finalize(m_get_node_tags_stmt);
    sqlite3_finalize(m_get_way_tags_stmt);
    sqlite3_finalize(m_get_relation_tags_stmt);
    sqlite3_close(m_db);     
  }

  //----------------------------------------------------------------------------
  void osm_cache::store(const osm_api_data_types::osm_node & p_node)
  {
    // Store node single part ( everything execept tags )
    osm_cache_node l_cache_node(p_node.get_id(),
				p_node.get_version(),
				p_node.get_user_id(),
				p_node.get_timestamp(),
				p_node.get_changeset(),
				p_node.get_lat(),
				p_node.get_lon()
				);
    uint32_t l_result = m_node_table.get_by_id_version(p_node.get_id(),
						       p_node.get_version(),
						       l_cache_node);

    store(p_node.get_user_id(),p_node.get_user(),"");

    if(!l_result)
      {
	m_node_table.force_create(l_cache_node);
      }

    // Store tags
    store_tags(m_node_tag_table,p_node);
  }

  //----------------------------------------------------------------------------
  void osm_cache::store(const osm_api_data_types::osm_way & p_way)
  {
    // Store way single part ( everything execept tags )
    osm_cache_core_element l_cache_way(p_way.get_id(),
                                       p_way.get_version(),
                                       p_way.get_user_id(),
                                       p_way.get_timestamp(),
                                       p_way.get_changeset()
                                       );
    uint32_t l_result = m_way_table.get_by_id_version(p_way.get_id(),
                                                      p_way.get_version(),
                                                      l_cache_way);

    store(p_way.get_user_id(),p_way.get_user(),"");

    if(!l_result)
      {
	m_way_table.force_create(l_cache_way);
      }

    // Store tags
    store_tags(m_way_tag_table,p_way);

    // Store related nodes
    for(uint32_t l_index = 0;
        l_index < p_way.get_node_refs().size();
        ++l_index)
      {
        osm_cache_way_member l_way_member(p_way.get_id(),
                                          p_way.get_version(),
                                          l_index,
                                          p_way.get_node_refs()[l_index]);
        uint32_t l_result = m_way_member_table.get_by_id_version_rank(p_way.get_id(),
                                                                      p_way.get_version(),
                                                                      l_index,
                                                                      l_way_member);
        if(!l_result)
          {
            m_way_member_table.force_create(l_way_member);
          }
      }
  }

  //----------------------------------------------------------------------------
  void osm_cache::store(const osm_api_data_types::osm_relation & p_relation)
  {
    // Store relation single part ( everything execept tags )
    osm_cache_core_element l_cache_relation(p_relation.get_id(),
					    p_relation.get_version(),
					    p_relation.get_user_id(),
					    p_relation.get_timestamp(),
					    p_relation.get_changeset()
					    );
    uint32_t l_result = m_relation_table.get_by_id_version(p_relation.get_id(),
							   p_relation.get_version(),
							   l_cache_relation);

    store(p_relation.get_user_id(),p_relation.get_user(),"");

    if(!l_result)
      {
	m_relation_table.force_create(l_cache_relation);
      }

    // Store tags
    store_tags(m_relation_tag_table,p_relation);

    // Store related nodes
    for(uint32_t l_index = 0;
        l_index < p_relation.get_members().size();
        ++l_index)
      {
	const osm_api_data_types::osm_relation_member * l_relation_member = p_relation.get_members()[l_index];
	// Cache role
	osm_cache_named_item l_cached_relation_role(l_relation_member->get_role());
	uint32_t l_role_result = m_relation_role_table.get_by_name(l_relation_member->get_role(),l_cached_relation_role);
	if(!l_role_result)m_relation_role_table.create(l_cached_relation_role);

	// Create cache representation
        osm_cache_relation_member l_cached_relation_member(p_relation.get_id(),
							   p_relation.get_version(),
							   l_index,
							   l_relation_member->get_object_ref(),
							   l_relation_member->get_type(),
							   l_cached_relation_role.get_id());

	// Search in cache
        uint32_t l_result = m_relation_member_table.get_by_id_version_rank(p_relation.get_id(),
									   p_relation.get_version(),
									   l_index,
									   l_cached_relation_member);
        if(!l_result)
          {
            m_relation_member_table.force_create(l_cached_relation_member);
          }
      }
  }

  //----------------------------------------------------------------------------
  void osm_cache::store(const osm_api_data_types::osm_object::t_osm_id & p_id,
			const std::string & p_user_name,
			const std::string & p_date)
  {
    osm_cache_user l_empty_user;
    uint32_t l_user_result = m_user_table.get(p_id,l_empty_user);
    if(!l_user_result)
      {
        osm_cache_user l_cache_user(p_id,p_user_name,p_date);
        m_user_table.force_create(l_cache_user);
      }
    else if(l_empty_user.get_date() == "" && p_date != "")
      {
        osm_cache_user l_cache_user(p_id,p_user_name,p_date);
        m_user_table.update(l_cache_user);
      }

   
  }

   //----------------------------------------------------------------------------
  bool osm_cache::load_user(const osm_api_data_types::osm_object::t_osm_id & p_id, std::string & p_name, std::string & p_date)
    {
      osm_cache_user l_empty_user;
      uint32_t l_user_result = m_user_table.get(p_id,l_empty_user);
      if(l_user_result)
        {
          p_name = l_empty_user.get_name();
          p_date = l_empty_user.get_date();
        }
      return l_user_result;
    }

  //----------------------------------------------------------------------------
  void osm_cache::store_tags(osm_cache_tag_table<osm_cache_object_tag> & p_table,
                             const osm_api_data_types::osm_core_element & p_element)
  {
    
    for(std::map<std::string,std::string>::const_iterator l_iter = p_element.get_tags().begin();
	l_iter != p_element.get_tags().end();
	++l_iter)
      {
	// Check if tag name is still existing in database
        //TO DELETE	osm_cache_tag_name l_cache_tag_name(l_iter->first);
	osm_cache_named_item l_cache_tag_name(l_iter->first);
	uint32_t l_name_result = m_tag_name_table.get_by_name(l_iter->first,l_cache_tag_name);
	if(!l_name_result)m_tag_name_table.create(l_cache_tag_name);

	// Check if tag value is still existing in database
        //TO DELETE	osm_cache_tag_value l_cache_tag_value(l_iter->second);
	osm_cache_named_item l_cache_tag_value(l_iter->second);
	uint64_t l_value_result = m_tag_value_table.get_by_name(l_iter->second,l_cache_tag_value);
	if(!l_value_result)m_tag_value_table.create(l_cache_tag_value);

        //TO DELETE	osm_cache_node_tag l_node_tag(p_node.get_id(),
	osm_cache_object_tag l_node_tag(p_element.get_id(),
                                        p_element.get_version(),
                                        l_cache_tag_name.get_id(),
                                        l_cache_tag_value.get_id()
                                        );
	uint32_t l_result = p_table.exists(l_node_tag);
	if(!l_result)
	  {
	    p_table.force_create(l_node_tag);
	  }
      }    
  }

  //----------------------------------------------------------------------------
  const osm_api_data_types::osm_node * const osm_cache::load_node(const osm_api_data_types::osm_object::t_osm_id & p_id,
								  const osm_api_data_types::osm_core_element::t_osm_version & p_version)
  {
    
    
    osm_cache_node l_cache_node;
    // Get node
    uint32_t l_result = m_node_table.get_by_id_version(p_id,p_version,l_cache_node);
    if(l_result)
      {
        osm_cache_user l_cache_user;
        m_user_table.get(l_cache_node.get_uid(),l_cache_user);
        osm_api_data_types::osm_node * const l_result = new osm_api_data_types::osm_node(l_cache_node.get_id(),
                                                                                         l_cache_node.get_lat(),
                                                                                         l_cache_node.get_lon(),
                                                                                         l_cache_node.get_timestamp(),
                                                                                         l_cache_node.get_version(),
                                                                                         l_cache_node.get_changeset(),
                                                                                         l_cache_node.get_uid(),
                                                                                         l_cache_user.get_name()
                                                                                         );
        std::vector<std::pair<std::string,std::string> > l_cache_node_tags;
        //TO DELETE        get_tags<osm_cache_node_tag>(m_get_node_tags_stmt,p_id,p_version,l_cache_node_tags);
        get_tags<osm_cache_object_tag>(m_get_node_tags_stmt,p_id,p_version,l_cache_node_tags);
        for(std::vector<std::pair<std::string,std::string> >::const_iterator l_iter = l_cache_node_tags.begin();
            l_iter != l_cache_node_tags.end();
            ++l_iter)
          {
            l_result->add_tag(l_iter->first,l_iter->second);
          }
        return l_result;
      }
    else
      {
        return NULL;
      }
  }

  //----------------------------------------------------------------------------
  const osm_api_data_types::osm_way * const osm_cache::load_way(const osm_api_data_types::osm_object::t_osm_id & p_id,
								const osm_api_data_types::osm_core_element::t_osm_version & p_version)
  {
    
    
    osm_cache_core_element l_cache_way;
    // Get way
    uint32_t l_result = m_way_table.get_by_id_version(p_id,p_version,l_cache_way);
    if(l_result)
      {
        osm_cache_user l_cache_user;
        m_user_table.get(l_cache_way.get_uid(),l_cache_user);
        osm_api_data_types::osm_way * const l_result = new osm_api_data_types::osm_way(l_cache_way.get_id(),
										       l_cache_way.get_timestamp(),
										       l_cache_way.get_version(),
										       l_cache_way.get_changeset(),
										       l_cache_way.get_uid(),
										       l_cache_user.get_name()
										       );
        // Get tags
        std::vector<std::pair<std::string,std::string> > l_cache_way_tags;
        //TO DELETE        get_tags<osm_cache_way_tag>(m_get_way_tags_stmt,p_id,p_version,l_cache_way_tags);
        get_tags<osm_cache_object_tag>(m_get_way_tags_stmt,p_id,p_version,l_cache_way_tags);
        for(std::vector<std::pair<std::string,std::string> >::const_iterator l_iter = l_cache_way_tags.begin();
            l_iter != l_cache_way_tags.end();
            ++l_iter)
          {
            l_result->add_tag(l_iter->first,l_iter->second);
          }

        // Get members
        std::vector<osm_cache_way_member> l_cached_way_members;
        m_way_member_table.get_by_id_version_ordered(p_id,p_version,l_cached_way_members);
        for(std::vector<osm_cache_way_member>::const_iterator l_iter = l_cached_way_members.begin();
            l_iter != l_cached_way_members.end();
            ++l_iter)
          {
            l_result->add_node(l_iter->get_ref());
          }
        return l_result;
      }
    else
      {
        return NULL;
      }
  }

  //----------------------------------------------------------------------------
  const osm_api_data_types::osm_relation * const osm_cache::load_relation(const osm_api_data_types::osm_object::t_osm_id & p_id,
									  const osm_api_data_types::osm_core_element::t_osm_version & p_version)
  {
    
    
    osm_cache_core_element l_cache_relation;
    // Get relation
    uint32_t l_result = m_relation_table.get_by_id_version(p_id,p_version,l_cache_relation);
    if(l_result)
      {
	// Get user display name
        osm_cache_user l_cache_user;
        m_user_table.get(l_cache_relation.get_uid(),l_cache_user);

	// Get relation
        osm_api_data_types::osm_relation * const l_result = new osm_api_data_types::osm_relation(l_cache_relation.get_id(),
												 l_cache_relation.get_timestamp(),
												 l_cache_relation.get_version(),
												 l_cache_relation.get_changeset(),
												 l_cache_relation.get_uid(),
												 l_cache_user.get_name()
												 );
        // Get tags
        std::vector<std::pair<std::string,std::string> > l_cache_relation_tags;
        get_tags<osm_cache_object_tag>(m_get_relation_tags_stmt,p_id,p_version,l_cache_relation_tags);
        for(std::vector<std::pair<std::string,std::string> >::const_iterator l_iter = l_cache_relation_tags.begin();
            l_iter != l_cache_relation_tags.end();
            ++l_iter)
          {
            l_result->add_tag(l_iter->first,l_iter->second);
          }

        // Get members
        std::vector<osm_api_data_types::osm_relation_member*> l_cached_relation_members;
        get_relation_members(p_id,p_version,l_cached_relation_members);
        for(std::vector<osm_api_data_types::osm_relation_member*>::const_iterator l_iter = l_cached_relation_members.begin();
            l_iter != l_cached_relation_members.end();
            ++l_iter)
          {
            l_result->add_member((*l_iter)->get_type(),(*l_iter)->get_object_ref(),(*l_iter)->get_role());
	    delete *l_iter;
          }
        return l_result;
      }
    else
      {
        return NULL;
      }
  }

  //----------------------------------------------------------------------------
  void osm_cache::get_relation_members(const osm_api_data_types::osm_object::t_osm_id & p_id,
				       const osm_api_data_types::osm_core_element::t_osm_version & p_version,
				       std::vector<osm_api_data_types::osm_relation_member *> & p_result)
  {
    // Binding values to statement
    //----------------------------

    int l_status = sqlite3_bind_int64(m_get_relation_members_stmt,sqlite3_bind_parameter_index(m_get_relation_members_stmt,"$id"),p_id);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during binding of id parameter for get_relation_members statement : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }
    
    l_status = sqlite3_bind_int(m_get_relation_members_stmt,sqlite3_bind_parameter_index(m_get_relation_members_stmt,"$version"),p_version);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during binding of version parameter for get_relation_members statement : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }
    // Executing statement
    //---------------------
    while((l_status = sqlite3_step(m_get_relation_members_stmt)) == SQLITE_ROW)
      {
	p_result.push_back(new osm_api_data_types::osm_relation_member((osm_api_data_types::osm_core_element::t_osm_type)sqlite3_column_int(m_get_relation_members_stmt,0),//Type
								       sqlite3_column_int64(m_get_relation_members_stmt,1),//Ref
								       (const char*)sqlite3_column_text(m_get_relation_members_stmt,2)//ROLE
								       )
			   );
      }
    if(l_status != SQLITE_DONE)
      {
	std::cout << "ERROR during selection of relation_members : " << sqlite3_errmsg(m_db) << std::endl ;
	exit(-1);
      }

    std::cout << "get_relation_members successfully listed" << std::endl ;

    // Reset the statement for the next use
    //--------------------------------------
    l_status = sqlite3_reset(m_get_relation_members_stmt);  
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during reset of get_relation_members statement : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }

    // Reset bindings because they are now useless
    //--------------------------------------------
#if SQLITE_VERSION_NUMBER > 3006000
    l_status = sqlite3_clear_bindings(m_get_relation_members_stmt);
    if(l_status != SQLITE_OK)
      {
	std::cout << "ERROR during reset of bindings of get_relation_members statement : " << sqlite3_errmsg(m_db) << std::endl ;     
	exit(-1);
      }
#endif
  
  }
}
//EOF
