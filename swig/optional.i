%include "std_common.i"
%include "typemaps.i"

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


    void value(T *out) const;
    void value(T *out);


    constexpr bool has_value() const noexcept;


  };
  template <class T>
  constexpr optional<T> make_optional(T &&arg);
}



%define template_optional_base(__type__, __name__)


%typemap(directorin) tl::optional<__type__> %{
    if($input == Py_None)
        $1 = __type__();
    else
        $1 = PyObject(tl::make_optional($input));
%}

%typemap(in, numinputs=0) __type__ *out (__type__ temp) {
  $1 = &temp;
}
%template(Optional ## __name__) tl::optional<__type__>;
%template() tl::optional<__type__ &>;
%{
  template tl::optional<__type__> tl::make_optional(__type__ &&);
%}
namespace tl{
  constexpr optional<__type__> make_optional(__type__ arg);
}
%enddef

%define template_optional_object(__type__, __name__)
%typemap(out) tl::optional<__type__ &> %{
  if((&$1)->has_value()){
      $result = SWIG_Python_AppendOutput($result, SWIG_NewPointerObj(SWIG_as_voidptr(&($1.value())), $descriptor(__type__ *), (SWIG_POINTER_NO_NULL | 0)));
  }
  else
  {
      $result = Py_None;
      Py_INCREF(Py_None);
  }
%}
%typemap(argout) __type__ *out {
  $result = SWIG_Python_AppendOutput($result, SWIG_NewPointerObj(SWIG_as_voidptr($1), $descriptor(__type__ *), (SWIG_POINTER_NO_NULL |  0 )));
}
template_optional_base(__type__, __name__);
%enddef


%define template_optional_integral(__type__, __name__)
%typemap(out) tl::optional<__type__ &> %{
  if((&$1)->has_value()){
      using T = std::remove_const<std::remove_reference<__type__>::type>::type;
      T *res ;
      res = (T *)new T();
      *res = (&$1)->value();
      $result = SWIG_From_int(*res)
  }
  else
  {
      $result = Py_None;
      Py_INCREF(Py_None);
  }
%}
%typemap(argout) __type__ *out {
  $result = SWIG_From_int(*$1);
}
template_optional_base(__type__, __name__);
%enddef

//     //   return SWIG_From_float(*value);
//     //   return SWIG_From_std_string(*value);
//     //   return SWIG_NewPointerObj(SWIG_as_voidptr(value), $descriptor(__type__ *), (SWIG_POINTER_OWN |  0 ));
