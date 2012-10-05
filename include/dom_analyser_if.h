#ifndef _DOM_ANALYSER_IF_H_
#define _DOM_ANALYSER_IF_H_

#include "dom_analyser_generic_if.h"
#include "xmlParser.h"

// Type defining interface of dom_analyser independantly of type used to represent the XML tree
typedef dom_analyser_generic_if<XMLNode> dom_analyser_if;
typedef XMLNode t_dom_tree;
#endif // _DOM_ANALYSER_IF_H_
//EOF
