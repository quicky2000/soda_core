#ifndef _OSM_DIFF_PARSER_BASE_H_
#define _OSM_DIFF_PARSER_BASE_H_

#include <set>
#include <algorithm>
#include <iostream>
#include "gzstream.h"
#include "analyse_operation.h"
#include "osm_diff_parser_if.h"

template <class T>
class osm_diff_parser_base:public osm_diff_parser_if
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

#include "osm_diff_parser_base.tpp"

#endif // _OSM_DIFF_PARSER_BASE_H_
