%include <std_common.i>

%{
#include "optional.hpp"
%}

namespace tl {
  template<typename T> struct optional {
    typedef T value_type;

    %traits_swigtype(T);

    optional();
    optional(T value);
    optional(const optional& other);
    template <class T> optional(const optional<T> &other);

    %apply T & OUTPUT { T &value };
    void get_value(T& OUTPUT) const;
    void get_value(T& OUTPUT);

    T &value() &;
    const T &value() const &;
    T &&value() &&;
    const T &&value() const &&;
    constexpr bool has_value() const noexcept;


    // T value;
  };
}

namespace swig {
  template <typename T>  struct traits<tl::optional< T > > {
    typedef pointer_category category;
    static const char* type_name() {
      return "tl::optional<" #T " >";
    }
  };
  template <class T> struct traits_asval<tl::optional<T> >  {
    typedef tl::optional<T> value_type;
    static int get_value(PyObject* value,  tl::optional<T> *val)
    {
      if (val) {
        T *value_ptr = &(val->value());
        int res = swig::asval((PyObject*)value, value_ptr);
        if (!SWIG_IsOK(res)) return res;
        return res;
      } else {
        T *value_ptr = 0;
        int res = swig::asval((PyObject*)value, 0);
        if (!SWIG_IsOK(res)) return res;
        return res;
      }
    }
    static int asval(PyObject *obj, tl::optional<T> *val) {
      int res = SWIG_ERROR;
      res = get_value(obj, val);
      if (res == SWIG_ERROR) {
        value_type *p = 0;
        swig_type_info *descriptor = swig::type_info<value_type>();
        res = descriptor ? SWIG_ConvertPtr(obj, (void **)&p, descriptor, 0) : SWIG_ERROR;
        if (SWIG_IsOK(res) && val)  *val = *p;
      }
      return res;

    }
  };
  // template <>  struct traits_asval< tl::optional< T > > {
  //   typedef std::remove_const<std::remove_reference<T>::type>::type value_type;
  //   static int asval(PyObject *obj, value_type *val) {
  //     return SWIG_AsVal_int (obj, (int *)val);
  //   }
  // };
  // template <>  struct traits_from< tl::optional< T > > {
  //   typedef std::remove_const<std::remove_reference<T>::type>::type value_type;
  //   static PyObject *from(const value_type& val) {
  //     return SWIG_From_int  ((int)val);
  //   }
  // };

}


%typemap(out) tl::optional<typename T> %{
  if((&$1)->has_value()){
      using T = std::remove_const<std::remove_reference<$1_ltype::value_type>::type>::type;
      T *res ;
      res = (T *)new T();
      *res = (&$1)->value();
      //$result = PyObject((&$1)->value());
      // $descriptor(T *)
      $result = SWIG_NewPointerObj(SWIG_as_voidptr(res), SWIGTYPE_p_ghr__MonsterActor, SWIG_POINTER_NEW | 0);
  }
  else
  {
      $result = Py_None;
      Py_INCREF(Py_None);
  }
%}

// %typemap(out) tl::optional<int> %{
//   if((&$1)->has_value()){
//       $result = PyLong_FromLong(*$1);
//   }
//   else
//   {
//       $result = Py_None;
//       Py_INCREF(Py_None);
//   }
// %}