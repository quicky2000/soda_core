#ifndef _DOM_ANALYSER_GENERIC_IF_H_
#define _DOM_ANALYSER_GENERIC_IF_H_

namespace osm_diff_watcher
{
  template <class T>
    class dom_analyser_generic_if
    {
    public:
      typedef T node_type;

      virtual void analyse(const T & p_tree)=0;
      virtual ~dom_analyser_generic_if(void){}
    };
}
#endif // _DOM_ANALYSER_GENERIC_IF_H_
//EOF
