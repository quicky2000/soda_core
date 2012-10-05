#ifndef _OSM_DIFF_PARSER_IF_H_
#define _OSM_DIFF_PARSER_IF_H_

#include <string>

class osm_diff_parser_if
{
 public:
  virtual void parse(const std::string & p_name)=0;
  inline ~osm_diff_parser_if(void){}
};

#endif // _OSM_DIFF_PARSER_IF_H_
//EOF
