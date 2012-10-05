#ifndef _BASIC_DOM_ANALYSER_H_
#define _BASIC_DOM_ANALYSER_H_

#include "dom_analyser_if.h"

class basic_dom_analyser: public dom_analyser_if
{
 public:
  basic_dom_analyser(void);
  void analyse(const t_dom_tree & p_tree);
 private:
  unsigned int m_level;
};
#endif // _BASIC_DOM_ANALYSER_H_
