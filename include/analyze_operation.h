#ifndef _ANALYZE_OPERATION_H_
#define _ANALYZE_OPERATION_H_

namespace osm_diff_watcher
{
  template <class T>
    class analyze_operation
    {
    public:
      virtual void operator()(T *)const=0;
      inline virtual ~analyze_operation(){}
    private:
    };
}
#endif // _ANALYZE_OPERATION_H_
//EOF
