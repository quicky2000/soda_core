#ifndef _PARSER_BASE_H_
#define _PARSER_BASE_H_

#include <set>
#include <algorithm>
#include <iostream>
#include "gzstream.h"
#include "analyse_operation.h"
#include "parser_if.h"

namespace osm_diff_watcher
{
  template <class T>
    class parser_base:public parser_if
  {
  public:
    void add_analyser(T & p_analyser);
    void parse(const std::string & p_name);
  protected:
    void perform_analyse(const analyse_operation<T> & f);
  private:
    virtual void parse_stream(std::istream & p_stream)=0;

    std::set<T*> m_analysers;
  }
  ;

#include "parser_base.tpp"
}
#endif // _PARSER_BASE_H_
//EOF
