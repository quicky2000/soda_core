#ifndef _USER_ANALYSER_H_
#define _USER_ANALYSER_H_

#include "sax_analyser_if.h"
#include "stdint.h"
#include <map>
#include <fstream>

class user_analyser:public sax_analyser_if
{
 public:
  user_analyser(void);
  ~user_analyser(void);
  // Methods inherited from sax_analyser_if
  void init(void);
  void start_element(const std::string & p_name);
  void get_attribute(const std::string & p_name,const std::string & p_value);
  void end_element(const std::string & p_name);
  // End of inherited methods
private:
  std::string get_user_inscription_date(const std::string & p_user)const;

  std::ofstream m_report;
  uint32_t m_uid;
  std::string m_user_name;
  std::map<uint32_t,std::string> m_users;
  std::map<std::string,uint32_t> m_months;
};

#endif
