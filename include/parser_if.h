#ifndef _PARSER_IF_H_
#define _PARSER_IF_H_

#include <string>

namespace osm_diff_watcher
{
  class parser_if
  {
  public:
    virtual void parse(const std::string & p_name)=0;
    inline virtual ~parser_if(void){}
  };
}
#endif // _PARSER_IF_H_
//EOF
