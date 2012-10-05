#ifndef _SAX_ANALYSER_IF_H_
#define _SAX_ANALYSER_IF_H_

#include <string>

namespace osm_diff_watcher
{
  class sax_analyser_if
  {
  public:
    virtual void init(void)=0;

    virtual void start_element(const std::string & p_name)=0;

    virtual void get_attribute(const std::string & p_name,
			       const std::string & p_value)=0;

    virtual void end_element(const std::string & p_name)=0;

    inline virtual ~sax_analyser_if(void){};
  };
}
#endif
//EOF
