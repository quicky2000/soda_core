#ifndef _PARSER_BASE_H_
#define _PARSER_BASE_H_

#include <set>
#include <algorithm>
#include <iostream>
#include "gzstream.h"
#include "analyze_operation.h"
#include "parser_if.h"
#include "output_producer_if.h"

namespace osm_diff_watcher
{
  template <class T>
    class parser_base:public output_producer_if<T>, public parser_if
  {
  public:
    void parse(const std::string & p_name);
  private:
    virtual void parse_stream(std::istream & p_stream)=0;

  }
  ;

#include "parser_base.tpp"
}
#endif // _PARSER_BASE_H_
//EOF
