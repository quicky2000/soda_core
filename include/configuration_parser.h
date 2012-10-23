#ifndef _CONFIGURATION_PARSER_H_
#define _CONFIGURATION_PARSER_H_

#include "dom_configuration_extractor.h"
#include "configuration.h"
#include "dom_parser.h"

namespace osm_diff_watcher
{
  class configuration_parser
  {
  public:
    static const configuration * parse(const std::string & p_file_name);
  private:
  };

  //----------------------------------------------------------------------------
  const configuration * configuration_parser::parse(const std::string & p_file_name)
  {
    dom_parser l_configuration_parser("osm_diff_watcher_configuration");
    dom_configuration_extractor l_extractor;
    l_configuration_parser.add_analyzer(l_extractor);
    l_configuration_parser.parse(p_file_name);
    return l_extractor.get_result();
  }

}
#endif //_CONFIGURATION_PARSER_H_
//EOF
