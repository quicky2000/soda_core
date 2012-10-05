#ifndef _ANALYSE_OPERATION_H_
#define _ANALYSE_OPERATION_H_

namespace osm_diff_watcher
{
  template <class T>
    class analyse_operation
    {
    public:
      virtual void operator()(T *)const=0;
      inline virtual ~analyse_operation(){}
    private:
    };
}
#endif // _ANALYSE_OPERATION_H_
//EOF
