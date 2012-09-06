#include "user_analyser.h"
#include "url_reader.h"
#include "download_buffer.h"
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include "assert.h"

using namespace quicky_url_reader;

//------------------------------------------------------------------------------
user_analyser::user_analyser(void):
  m_report("new_user.txt")
{
  if(m_report == NULL)
    {
      std::cout << "Error when trying to open file \"new_user.txt\"" << std::endl ;
      exit(EXIT_FAILURE);
    }
  m_report << "Contributors since less than one month : "  << std::endl ;

  m_months.insert(std::map<std::string,uint32_t>::value_type("January",1));
  m_months.insert(std::map<std::string,uint32_t>::value_type("February",2));
  m_months.insert(std::map<std::string,uint32_t>::value_type("March",3));
  m_months.insert(std::map<std::string,uint32_t>::value_type("April",4));
  m_months.insert(std::map<std::string,uint32_t>::value_type("May",5));
  m_months.insert(std::map<std::string,uint32_t>::value_type("June",6));
  m_months.insert(std::map<std::string,uint32_t>::value_type("July",7));
  m_months.insert(std::map<std::string,uint32_t>::value_type("August",8));
  m_months.insert(std::map<std::string,uint32_t>::value_type("September",9));
  m_months.insert(std::map<std::string,uint32_t>::value_type("October",10));
  m_months.insert(std::map<std::string,uint32_t>::value_type("November",11));
  m_months.insert(std::map<std::string,uint32_t>::value_type("December",12));
}

//------------------------------------------------------------------------------
user_analyser::~user_analyser(void)
{
  m_report.close();
}

//------------------------------------------------------------------------------
void user_analyser::init(void)
{
}

//------------------------------------------------------------------------------
void user_analyser::start_element(const std::string & p_name)
{
  m_uid = 0;
  m_user_name = "";
}

//------------------------------------------------------------------------------
void user_analyser::end_element(const std::string & p_name)
{
  if(m_uid != 0 && m_user_name != "")
    {
      // Check if user has still been detected
      std::map<uint32_t,std::string>::const_iterator l_iter = m_users.find(m_uid);
      if(l_iter == m_users.end())
	{
	  std::string l_date = get_user_inscription_date(m_user_name);
	  m_users.insert(std::map<uint32_t,std::string>::value_type(m_uid,l_date));
	  std::cout << "==> User \"" << m_user_name << "\"\t\"" << l_date << "\"" << std::endl ;
	  size_t l_first_space = l_date.find(" ");
	  size_t l_second_sapce = l_date.find_last_of(" ");
	  std::string l_day_str = l_date.substr(0,l_first_space);
	  std::string l_month_str = l_date.substr(l_first_space+1,l_second_sapce-l_first_space-1);
	  std::string l_year_str = l_date.substr(l_second_sapce+1);
#ifdef DEBUG_USER_ANALYSER
	  std::cout << "Day = \"" << l_day_str << "\"" << std::endl ;
	  std::cout << "Month = \"" << l_month_str << "\"" << std::endl ;
	  std::cout << "Year = \"" << l_year_str << "\"" << std::endl ;
#endif
	  std::map<std::string,uint32_t>::const_iterator l_iter_month = m_months.find(l_month_str);
	  assert(m_months.end() != l_iter_month);
    
	  uint32_t l_inscription_day = atoi(l_day_str.c_str());   
	  uint32_t l_inscription_month  = l_iter_month->second  ;
	  uint32_t l_inscritpion_year = atoi(l_year_str.c_str()) ; 
	  time_t l_today = time(NULL);
	  struct tm l_inscription;
    
	  l_inscription.tm_mday = l_inscription_day;   /* 1er                */
	  l_inscription.tm_mon  = l_inscription_month - 1;   /* janvier            */
	  l_inscription.tm_year = l_inscritpion_year - 1900; /* 2000 (2000 - 1900) */
	  l_inscription.tm_hour = 0;   /* 0 heure            */
	  l_inscription.tm_min  = 0;   /* 0 minute           */
	  l_inscription.tm_sec  = 0;   /* 0 seconde          */

	  time_t l_inscription_date = mktime(&l_inscription);

	  time_t l_duration_time = difftime(l_today, l_inscription_date);
	  struct tm * l_duration = localtime(&l_duration_time);
	  std::cout << (l_duration->tm_year - 70 ) << " year " << (l_duration->tm_mon ) << " months " << l_duration->tm_mday << " days" << std::endl ;
	  if((l_duration->tm_year - 70 )== 0 && l_duration->tm_mon == 0)
	    {
	      m_report << "\"" << m_user_name << "\" : " << l_date << std::endl ;
	    } 
	}
    }
}

//------------------------------------------------------------------------------
void user_analyser::get_attribute(const std::string & p_name,const std::string & p_value)
{
  if(p_name == "uid") m_uid = atoi(p_value.c_str());
  if(p_name == "user") m_user_name = p_value;
}

//------------------------------------------------------------------------------
std::string user_analyser::get_user_inscription_date(const std::string & p_user)const
{
  std::string l_user = p_user;
  std::string l_user_date;
  std::string l_user_url("http://www.openstreetmap.org/user/");
  url_reader & l_url_reader = url_reader::get_instance();
  char * l_escaped_user = l_url_reader.escape_string(l_user.c_str());
  l_user_url += l_escaped_user;
  download_buffer l_buffer;
  l_url_reader.read_url(l_user_url.c_str(),l_buffer);
  curl_free(l_escaped_user);

  std::stringstream l_stream;
  l_stream << l_buffer.get_data();
  std::string l_line;
  bool l_found = false;
  while(!l_found && !getline(l_stream,l_line).eof())
    {
      size_t l_pos ;
      if((l_pos = l_line.find("Mapper since")) != std::string::npos)
	{
	  size_t l_begin = l_line.find(">",l_pos);
	  l_begin = l_line.find_first_not_of(" ",l_begin+1);
	  size_t l_end = l_line.find_last_of("<");
	  l_line = l_line.substr(l_begin,l_end - l_begin);
	  l_user_date = l_line.substr(0,l_line.find(" at"));
	  l_found = true;
	}
    }
  assert(l_found);
  return l_user_date;
}
//EOF
