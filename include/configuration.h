#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include <string>
#include <vector>
#include <map>

namespace osm_diff_watcher
{
  class configuration
  {
  public:
    inline configuration(void);
    inline void add_library(const std::string & p_name);
    inline void add_module(const std::string & p_name,const std::string & p_type);
    inline void add_variable(const std::string & p_name,const std::string & p_value);
    inline const std::vector<std::string> & get_libraries(void)const;
    inline const std::vector<std::pair<std::string,std::string> > & get_modules(void)const;
    inline const std::map<std::string,std::string> & get_variables(void)const;
    bool is_variable_defined(const std::string & p_name)const;
    const std::string get_variable(const std::string & p_name)const;
  private:
    std::vector<std::string> m_libraries;
    std::vector<std::pair<std::string,std::string> > m_modules;
    std::map<std::string,std::string> m_variables;
  };

  //----------------------------------------------------------------------------
  configuration::configuration(void)
    {
    }

  //----------------------------------------------------------------------------
  void configuration::add_library(const std::string & p_name)
  {
    m_libraries.push_back(p_name);
  }

  //----------------------------------------------------------------------------
  void configuration::add_module(const std::string & p_name,const std::string & p_type)
  {
    m_modules.push_back(std::pair<std::string,std::string>(p_name,p_type));
  }

  //----------------------------------------------------------------------------
  void configuration::add_variable(const std::string & p_name,const std::string & p_value)
  {
    m_variables.insert(make_pair(p_name,p_value));
  }

  //----------------------------------------------------------------------------
  const std::vector<std::pair<std::string,std::string> > & configuration::get_modules(void)const
    {
      return m_modules;
    }

  //----------------------------------------------------------------------------
  const std::map<std::string,std::string> & configuration::get_variables(void)const
    {
      return m_variables;
    }

  //----------------------------------------------------------------------------
  bool configuration::is_variable_defined(const std::string & p_name)const
  {
    return m_variables.find(p_name) != m_variables.end();
  }

  //----------------------------------------------------------------------------
  const std::string configuration::get_variable(const std::string & p_name)const
    {
      std::map<std::string,std::string>::const_iterator l_iter = m_variables.find(p_name);
      return (l_iter != m_variables.end() ? l_iter->second : "");
    }

  //----------------------------------------------------------------------------
  const std::vector<std::string> & configuration::get_libraries(void)const
    {
      return m_libraries;
    }
}
#endif // _CONFIGURATION_H_
//EOF
