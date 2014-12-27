#ifndef CALLBACK_H_
#define CALLBACK_H_

#include <stddef.h>
#include <tr1/type_traits>
#include "google/protobuf/service.h"

template <typename Type, bool Delete>
class CallbackAutoDeleter {
 public:
  explicit CallbackAutoDeleter(Type* callback) : callback_(callback) {}
  ~CallbackAutoDeleter() { if (Delete) delete callback_; }
 private:
  Type* callback_;
};

// Primary template
template <typename Signature>
class Callback;

template <>
class Callback<void()> : public ::google::protobuf::Closure {
 public:
  virtual ~Callback() {}
  virtual bool IsRepeatable() const = 0;
  virtual void Run() = 0;
  Callback *volatile next_closure;
 protected:
  Callback() : next_closure(NULL) {}
};

typedef Callback<void()> Closure;

// Specified for 0 arguments
template <
  typename R >
class Callback<R()> {
public:
  virtual ~Callback() {}
  virtual bool IsRepeatable() const = 0;
  virtual R Run() = 0;
};

// Specified for 1 arguments
template <
  typename R , typename A1>
class Callback<R(A1)> {
public:
  virtual ~Callback() {}
  virtual bool IsRepeatable() const = 0;
  virtual R Run(A1 a1) = 0;
};

// Specified for 2 arguments
template <
  typename R , typename A1, typename A2>
class Callback<R(A1, A2)> {
public:
  virtual ~Callback() {}
  virtual bool IsRepeatable() const = 0;
  virtual R Run(A1 a1, A2 a2) = 0;
};

// Specified for 3 arguments
template <
  typename R , typename A1, typename A2, typename A3>
class Callback<R(A1, A2, A3)> {
public:
  virtual ~Callback() {}
  virtual bool IsRepeatable() const = 0;
  virtual R Run(A1 a1, A2 a2, A3 a3) = 0;
};

// Specified for 4 arguments
template <
  typename R , typename A1, typename A2, typename A3, typename A4>
class Callback<R(A1, A2, A3, A4)> {
public:
  virtual ~Callback() {}
  virtual bool IsRepeatable() const = 0;
  virtual R Run(A1 a1, A2 a2, A3 a3, A4 a4) = 0;
};

// Specified for 5 arguments
template <
  typename R , typename A1, typename A2, typename A3, typename A4, typename A5>
class Callback<R(A1, A2, A3, A4, A5)> {
public:
  virtual ~Callback() {}
  virtual bool IsRepeatable() const = 0;
  virtual R Run(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) = 0;
};

// Specified for 6 arguments
template <
  typename R , typename A1, typename A2, typename A3, typename A4, typename A5,
      typename A6>
class Callback<R(A1, A2, A3, A4, A5, A6)> {
public:
  virtual ~Callback() {}
  virtual bool IsRepeatable() const = 0;
  virtual R Run(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) = 0;
};

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass >
class MemberCallback_0_0 : public Callback<R()> {
  typedef R (CallerClass::*MemberSignature)();
 public:
  MemberCallback_0_0(Class *object, MemberSignature member):
      object_(object), member_(member) {}
  ~MemberCallback_0_0() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<MemberCallback_0_0, SelfDelete> self_deleter(this);
    return (object_->*member_)();
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
};

template <typename R, typename Class, typename CallerClass>
Callback<R()>*
NewCallback(Class *object, R (CallerClass::*member)()) {
  return new MemberCallback_0_0<true, R, Class, CallerClass>(
      object, member);
}

template <typename R, typename Class, typename CallerClass>
Callback<R()>*
NewPermanentCallback(Class *object, R (CallerClass::*member)()) {
  return new MemberCallback_0_0<false, R, Class, CallerClass>(
      object, member);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass >
class ConstMemberCallback_0_0 : public Callback<R()> {
  typedef R (CallerClass::*MemberSignature)() const;
 public:
  ConstMemberCallback_0_0(const Class *object, MemberSignature member):
      object_(object), member_(member) {}
  ~ConstMemberCallback_0_0() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<ConstMemberCallback_0_0, SelfDelete> self_deleter(this);
    return (object_->*member_)();
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
};

template <typename R, typename Class, typename CallerClass>
Callback<R()>*
NewCallback(const Class *object, R (CallerClass::*member)() const) {
  return new ConstMemberCallback_0_0<true, R, Class, CallerClass>(
      object, member);
}

template <typename R, typename Class, typename CallerClass>
Callback<R()>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)() const) {
  return new ConstMemberCallback_0_0<false, R, Class, CallerClass>(
      object, member);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R>
class FunctionCallback_0_0 : public Callback<R()> {
  typedef R (*FunctionType)();
 public:
  FunctionCallback_0_0(FunctionType function) :
      function_(function) {}
  FunctionCallback_0_0() {
    function_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<FunctionCallback_0_0, SelfDelete> self_deleter(this);
    return function_();
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
};

template <typename R>
Callback<R()>*
NewCallback(R (*function)()) {
  return new FunctionCallback_0_0<true, R>(function);
}

template <typename R>
Callback<R()>*
NewPermanentCallback(R (*function)()) {
  return new FunctionCallback_0_0<false, R>(function);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1 , typename PreArg1>
class MemberCallback_0_1 : public Callback<R()> {
  typedef R (CallerClass::*MemberSignature)(Arg1);
 public:
  MemberCallback_0_1(Class *object, MemberSignature member, PreArg1 pa1):
      object_(object), member_(member), pa_1(pa1) {}
  ~MemberCallback_0_1() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<MemberCallback_0_1, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename PreArg1>
Callback<R()>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1), PreArg1 pa1) {
  return new MemberCallback_0_1<true, R, Class, CallerClass, Arg1, PreArg1>(
      object, member, pa1);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename PreArg1>
Callback<R()>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1),
    PreArg1 pa1) {
  return new MemberCallback_0_1<false, R, Class, CallerClass, Arg1, PreArg1>(
      object, member, pa1);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1 , typename PreArg1>
class ConstMemberCallback_0_1 : public Callback<R()> {
  typedef R (CallerClass::*MemberSignature)(Arg1) const;
 public:
  ConstMemberCallback_0_1(const Class *object, MemberSignature member,
      PreArg1 pa1):
      object_(object), member_(member), pa_1(pa1) {}
  ~ConstMemberCallback_0_1() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<ConstMemberCallback_0_1, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename PreArg1>
Callback<R()>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1) const,
    PreArg1 pa1) {
  return new ConstMemberCallback_0_1<true, R, Class, CallerClass, Arg1,
      PreArg1>(
      object, member, pa1);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename PreArg1>
Callback<R()>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1) const,
    PreArg1 pa1) {
  return new ConstMemberCallback_0_1<false, R, Class, CallerClass, Arg1,
      PreArg1>(
      object, member, pa1);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename PreArg1>
class FunctionCallback_0_1 : public Callback<R()> {
  typedef R (*FunctionType)(Arg1);
 public:
  FunctionCallback_0_1(FunctionType function, PreArg1 pa1) :
      function_(function), pa_1(pa1) {}
  FunctionCallback_0_1() {
    function_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<FunctionCallback_0_1, SelfDelete> self_deleter(this);
    return function_(pa_1);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Arg1, typename PreArg1>
Callback<R()>*
NewCallback(R (*function)(Arg1), PreArg1 pa1) {
  return new FunctionCallback_0_1<true, R, Arg1, PreArg1>(function, pa1);
}

template <typename R, typename Arg1, typename PreArg1>
Callback<R()>*
NewPermanentCallback(R (*function)(Arg1), PreArg1 pa1) {
  return new FunctionCallback_0_1<false, R, Arg1, PreArg1>(function, pa1);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2 , typename PreArg1,
      typename PreArg2>
class MemberCallback_0_2 : public Callback<R()> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2);
 public:
  MemberCallback_0_2(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2) {}
  ~MemberCallback_0_2() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<MemberCallback_0_2, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename PreArg1, typename PreArg2>
Callback<R()>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2), PreArg1 pa1,
    PreArg2 pa2) {
  return new MemberCallback_0_2<true, R, Class, CallerClass, Arg1, Arg2,
      PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename PreArg1, typename PreArg2>
Callback<R()>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2),
    PreArg1 pa1, PreArg2 pa2) {
  return new MemberCallback_0_2<false, R, Class, CallerClass, Arg1, Arg2,
      PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2 , typename PreArg1,
      typename PreArg2>
class ConstMemberCallback_0_2 : public Callback<R()> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2) const;
 public:
  ConstMemberCallback_0_2(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2) {}
  ~ConstMemberCallback_0_2() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<ConstMemberCallback_0_2, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename PreArg1, typename PreArg2>
Callback<R()>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2) const,
    PreArg1 pa1, PreArg2 pa2) {
  return new ConstMemberCallback_0_2<true, R, Class, CallerClass, Arg1, Arg2,
      PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename PreArg1, typename PreArg2>
Callback<R()>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1,
    Arg2) const, PreArg1 pa1, PreArg2 pa2) {
  return new ConstMemberCallback_0_2<false, R, Class, CallerClass, Arg1, Arg2,
      PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename PreArg1, typename PreArg2>
class FunctionCallback_0_2 : public Callback<R()> {
  typedef R (*FunctionType)(Arg1, Arg2);
 public:
  FunctionCallback_0_2(FunctionType function, PreArg1 pa1, PreArg2 pa2) :
      function_(function), pa_1(pa1), pa_2(pa2) {}
  FunctionCallback_0_2() {
    function_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<FunctionCallback_0_2, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Arg1, typename Arg2, typename PreArg1,
    typename PreArg2>
Callback<R()>*
NewCallback(R (*function)(Arg1, Arg2), PreArg1 pa1, PreArg2 pa2) {
  return new FunctionCallback_0_2<true, R, Arg1, Arg2, PreArg1,
      PreArg2>(function, pa1, pa2);
}

template <typename R, typename Arg1, typename Arg2, typename PreArg1,
    typename PreArg2>
Callback<R()>*
NewPermanentCallback(R (*function)(Arg1, Arg2), PreArg1 pa1, PreArg2 pa2) {
  return new FunctionCallback_0_2<false, R, Arg1, Arg2, PreArg1,
      PreArg2>(function, pa1, pa2);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3 ,
      typename PreArg1, typename PreArg2, typename PreArg3>
class MemberCallback_0_3 : public Callback<R()> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3);
 public:
  MemberCallback_0_3(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  ~MemberCallback_0_3() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<MemberCallback_0_3, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename PreArg1, typename PreArg2,
    typename PreArg3>
Callback<R()>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new MemberCallback_0_3<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename PreArg1, typename PreArg2,
    typename PreArg3>
Callback<R()>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new MemberCallback_0_3<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3 ,
      typename PreArg1, typename PreArg2, typename PreArg3>
class ConstMemberCallback_0_3 : public Callback<R()> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3) const;
 public:
  ConstMemberCallback_0_3(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  ~ConstMemberCallback_0_3() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<ConstMemberCallback_0_3, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename PreArg1, typename PreArg2,
    typename PreArg3>
Callback<R()>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new ConstMemberCallback_0_3<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename PreArg1, typename PreArg2,
    typename PreArg3>
Callback<R()>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new ConstMemberCallback_0_3<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename PreArg1,
      typename PreArg2, typename PreArg3>
class FunctionCallback_0_3 : public Callback<R()> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3);
 public:
  FunctionCallback_0_3(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  FunctionCallback_0_3() {
    function_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<FunctionCallback_0_3, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R()>*
NewCallback(R (*function)(Arg1, Arg2, Arg3), PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3) {
  return new FunctionCallback_0_3<true, R, Arg1, Arg2, Arg3, PreArg1, PreArg2,
      PreArg3>(function, pa1, pa2, pa3);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R()>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3), PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3) {
  return new FunctionCallback_0_3<false, R, Arg1, Arg2, Arg3, PreArg1, PreArg2,
      PreArg3>(function, pa1, pa2, pa3);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4 , typename PreArg1, typename PreArg2, typename PreArg3,
      typename PreArg4>
class MemberCallback_0_4 : public Callback<R()> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4);
 public:
  MemberCallback_0_4(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4) {}
  ~MemberCallback_0_4() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<MemberCallback_0_4, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R()>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new MemberCallback_0_4<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R()>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new MemberCallback_0_4<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4 , typename PreArg1, typename PreArg2, typename PreArg3,
      typename PreArg4>
class ConstMemberCallback_0_4 : public Callback<R()> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4) const;
 public:
  ConstMemberCallback_0_4(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4) {}
  ~ConstMemberCallback_0_4() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<ConstMemberCallback_0_4, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R()>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new ConstMemberCallback_0_4<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R()>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new ConstMemberCallback_0_4<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
class FunctionCallback_0_4 : public Callback<R()> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4);
 public:
  FunctionCallback_0_4(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4) {}
  FunctionCallback_0_4() {
    function_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<FunctionCallback_0_4, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4>
Callback<R()>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4), PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4) {
  return new FunctionCallback_0_4<true, R, Arg1, Arg2, Arg3, Arg4, PreArg1,
      PreArg2, PreArg3, PreArg4>(function, pa1, pa2, pa3, pa4);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4>
Callback<R()>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4), PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new FunctionCallback_0_4<false, R, Arg1, Arg2, Arg3, Arg4, PreArg1,
      PreArg2, PreArg3, PreArg4>(function, pa1, pa2, pa3, pa4);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5 , typename PreArg1, typename PreArg2,
      typename PreArg3, typename PreArg4, typename PreArg5>
class MemberCallback_0_5 : public Callback<R()> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5);
 public:
  MemberCallback_0_5(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5) {}
  ~MemberCallback_0_5() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<MemberCallback_0_5, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5>
Callback<R()>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new MemberCallback_0_5<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5>
Callback<R()>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4,
    PreArg5 pa5) {
  return new MemberCallback_0_5<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5 , typename PreArg1, typename PreArg2,
      typename PreArg3, typename PreArg4, typename PreArg5>
class ConstMemberCallback_0_5 : public Callback<R()> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5) const;
 public:
  ConstMemberCallback_0_5(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5) {}
  ~ConstMemberCallback_0_5() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<ConstMemberCallback_0_5, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5>
Callback<R()>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4,
    PreArg5 pa5) {
  return new ConstMemberCallback_0_5<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5>
Callback<R()>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5) {
  return new ConstMemberCallback_0_5<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename PreArg1, typename PreArg2, typename PreArg3,
      typename PreArg4, typename PreArg5>
class FunctionCallback_0_5 : public Callback<R()> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5);
 public:
  FunctionCallback_0_5(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4),
          pa_5(pa5) {}
  FunctionCallback_0_5() {
    function_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<FunctionCallback_0_5, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, pa_5);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4, typename PreArg5>
Callback<R()>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5), PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new FunctionCallback_0_5<true, R, Arg1, Arg2, Arg3, Arg4, Arg5,
      PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(function, pa1, pa2, pa3,
      pa4, pa5);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4, typename PreArg5>
Callback<R()>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5), PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new FunctionCallback_0_5<false, R, Arg1, Arg2, Arg3, Arg4, Arg5,
      PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(function, pa1, pa2, pa3,
      pa4, pa5);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6 , typename PreArg1,
      typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5,
      typename PreArg6>
class MemberCallback_0_6 : public Callback<R()> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
 public:
  MemberCallback_0_6(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5), pa_6(pa6) {}
  ~MemberCallback_0_6() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<MemberCallback_0_6, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5, typename PreArg6>
Callback<R()>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4,
    PreArg5 pa5, PreArg6 pa6) {
  return new MemberCallback_0_6<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5, typename PreArg6>
Callback<R()>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4,
    PreArg5 pa5, PreArg6 pa6) {
  return new MemberCallback_0_6<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6 , typename PreArg1,
      typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5,
      typename PreArg6>
class ConstMemberCallback_0_6 : public Callback<R()> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5,
      Arg6) const;
 public:
  ConstMemberCallback_0_6(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5,
      PreArg6 pa6):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5), pa_6(pa6) {}
  ~ConstMemberCallback_0_6() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<ConstMemberCallback_0_6, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5, typename PreArg6>
Callback<R()>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new ConstMemberCallback_0_6<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5,
      PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5, typename PreArg6>
Callback<R()>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new ConstMemberCallback_0_6<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5,
      PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename PreArg1, typename PreArg2,
      typename PreArg3, typename PreArg4, typename PreArg5, typename PreArg6>
class FunctionCallback_0_6 : public Callback<R()> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
 public:
  FunctionCallback_0_6(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4),
          pa_5(pa5), pa_6(pa6) {}
  FunctionCallback_0_6() {
    function_ = NULL;
  }
  virtual R Run() {
    CallbackAutoDeleter<FunctionCallback_0_6, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5,
    typename PreArg6>
Callback<R()>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new FunctionCallback_0_6<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      PreArg1, PreArg2, PreArg3, PreArg4, PreArg5, PreArg6>(function, pa1, pa2,
      pa3, pa4, pa5, pa6);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5,
    typename PreArg6>
Callback<R()>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5,
    PreArg6 pa6) {
  return new FunctionCallback_0_6<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      PreArg1, PreArg2, PreArg3, PreArg4, PreArg5, PreArg6>(function, pa1, pa2,
      pa3, pa4, pa5, pa6);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1 >
class MemberCallback_1_0 : public Callback<R(Arg1)> {
  typedef R (CallerClass::*MemberSignature)(Arg1);
 public:
  MemberCallback_1_0(Class *object, MemberSignature member):
      object_(object), member_(member) {}
  ~MemberCallback_1_0() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg1 arg1) {
    CallbackAutoDeleter<MemberCallback_1_0, SelfDelete> self_deleter(this);
    return (object_->*member_)(arg1);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
};

template <typename R, typename Class, typename CallerClass, typename Arg1>
Callback<R(Arg1)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1)) {
  return new MemberCallback_1_0<true, R, Class, CallerClass, Arg1>(
      object, member);
}

template <typename R, typename Class, typename CallerClass, typename Arg1>
Callback<R(Arg1)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1)) {
  return new MemberCallback_1_0<false, R, Class, CallerClass, Arg1>(
      object, member);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1 >
class ConstMemberCallback_1_0 : public Callback<R(Arg1)> {
  typedef R (CallerClass::*MemberSignature)(Arg1) const;
 public:
  ConstMemberCallback_1_0(const Class *object, MemberSignature member):
      object_(object), member_(member) {}
  ~ConstMemberCallback_1_0() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg1 arg1) {
    CallbackAutoDeleter<ConstMemberCallback_1_0, SelfDelete> self_deleter(this);
    return (object_->*member_)(arg1);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
};

template <typename R, typename Class, typename CallerClass, typename Arg1>
Callback<R(Arg1)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1) const) {
  return new ConstMemberCallback_1_0<true, R, Class, CallerClass, Arg1>(
      object, member);
}

template <typename R, typename Class, typename CallerClass, typename Arg1>
Callback<R(Arg1)>*
NewPermanentCallback(const Class *object,
    R (CallerClass::*member)(Arg1) const) {
  return new ConstMemberCallback_1_0<false, R, Class, CallerClass, Arg1>(
      object, member);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1>
class FunctionCallback_1_0 : public Callback<R(Arg1)> {
  typedef R (*FunctionType)(Arg1);
 public:
  FunctionCallback_1_0(FunctionType function) :
      function_(function) {}
  FunctionCallback_1_0() {
    function_ = NULL;
  }
  virtual R Run(Arg1 arg1) {
    CallbackAutoDeleter<FunctionCallback_1_0, SelfDelete> self_deleter(this);
    return function_(arg1);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
};

template <typename R, typename Arg1>
Callback<R(Arg1)>*
NewCallback(R (*function)(Arg1)) {
  return new FunctionCallback_1_0<true, R, Arg1>(function);
}

template <typename R, typename Arg1>
Callback<R(Arg1)>*
NewPermanentCallback(R (*function)(Arg1)) {
  return new FunctionCallback_1_0<false, R, Arg1>(function);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2 , typename PreArg1>
class MemberCallback_1_1 : public Callback<R(Arg2)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2);
 public:
  MemberCallback_1_1(Class *object, MemberSignature member, PreArg1 pa1):
      object_(object), member_(member), pa_1(pa1) {}
  ~MemberCallback_1_1() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg2 arg2) {
    CallbackAutoDeleter<MemberCallback_1_1, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, arg2);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename PreArg1>
Callback<R(Arg2)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2), PreArg1 pa1) {
  return new MemberCallback_1_1<true, R, Class, CallerClass, Arg1, Arg2,
      PreArg1>(
      object, member, pa1);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename PreArg1>
Callback<R(Arg2)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2),
    PreArg1 pa1) {
  return new MemberCallback_1_1<false, R, Class, CallerClass, Arg1, Arg2,
      PreArg1>(
      object, member, pa1);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2 , typename PreArg1>
class ConstMemberCallback_1_1 : public Callback<R(Arg2)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2) const;
 public:
  ConstMemberCallback_1_1(const Class *object, MemberSignature member,
      PreArg1 pa1):
      object_(object), member_(member), pa_1(pa1) {}
  ~ConstMemberCallback_1_1() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg2 arg2) {
    CallbackAutoDeleter<ConstMemberCallback_1_1, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, arg2);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename PreArg1>
Callback<R(Arg2)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2) const,
    PreArg1 pa1) {
  return new ConstMemberCallback_1_1<true, R, Class, CallerClass, Arg1, Arg2,
      PreArg1>(
      object, member, pa1);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename PreArg1>
Callback<R(Arg2)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1,
    Arg2) const, PreArg1 pa1) {
  return new ConstMemberCallback_1_1<false, R, Class, CallerClass, Arg1, Arg2,
      PreArg1>(
      object, member, pa1);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename PreArg1>
class FunctionCallback_1_1 : public Callback<R(Arg2)> {
  typedef R (*FunctionType)(Arg1, Arg2);
 public:
  FunctionCallback_1_1(FunctionType function, PreArg1 pa1) :
      function_(function), pa_1(pa1) {}
  FunctionCallback_1_1() {
    function_ = NULL;
  }
  virtual R Run(Arg2 arg2) {
    CallbackAutoDeleter<FunctionCallback_1_1, SelfDelete> self_deleter(this);
    return function_(pa_1, arg2);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Arg1, typename Arg2, typename PreArg1>
Callback<R(Arg2)>*
NewCallback(R (*function)(Arg1, Arg2), PreArg1 pa1) {
  return new FunctionCallback_1_1<true, R, Arg1, Arg2, PreArg1>(function, pa1);
}

template <typename R, typename Arg1, typename Arg2, typename PreArg1>
Callback<R(Arg2)>*
NewPermanentCallback(R (*function)(Arg1, Arg2), PreArg1 pa1) {
  return new FunctionCallback_1_1<false, R, Arg1, Arg2, PreArg1>(function, pa1);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3 ,
      typename PreArg1, typename PreArg2>
class MemberCallback_1_2 : public Callback<R(Arg3)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3);
 public:
  MemberCallback_1_2(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2) {}
  ~MemberCallback_1_2() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg3 arg3) {
    CallbackAutoDeleter<MemberCallback_1_2, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, arg3);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename PreArg1, typename PreArg2>
Callback<R(Arg3)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3),
    PreArg1 pa1, PreArg2 pa2) {
  return new MemberCallback_1_2<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename PreArg1, typename PreArg2>
Callback<R(Arg3)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3),
    PreArg1 pa1, PreArg2 pa2) {
  return new MemberCallback_1_2<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3 ,
      typename PreArg1, typename PreArg2>
class ConstMemberCallback_1_2 : public Callback<R(Arg3)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3) const;
 public:
  ConstMemberCallback_1_2(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2) {}
  ~ConstMemberCallback_1_2() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg3 arg3) {
    CallbackAutoDeleter<ConstMemberCallback_1_2, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, arg3);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename PreArg1, typename PreArg2>
Callback<R(Arg3)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3) const, PreArg1 pa1, PreArg2 pa2) {
  return new ConstMemberCallback_1_2<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename PreArg1, typename PreArg2>
Callback<R(Arg3)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3) const, PreArg1 pa1, PreArg2 pa2) {
  return new ConstMemberCallback_1_2<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename PreArg1,
      typename PreArg2>
class FunctionCallback_1_2 : public Callback<R(Arg3)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3);
 public:
  FunctionCallback_1_2(FunctionType function, PreArg1 pa1, PreArg2 pa2) :
      function_(function), pa_1(pa1), pa_2(pa2) {}
  FunctionCallback_1_2() {
    function_ = NULL;
  }
  virtual R Run(Arg3 arg3) {
    CallbackAutoDeleter<FunctionCallback_1_2, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, arg3);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename PreArg1, typename PreArg2>
Callback<R(Arg3)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3), PreArg1 pa1, PreArg2 pa2) {
  return new FunctionCallback_1_2<true, R, Arg1, Arg2, Arg3, PreArg1,
      PreArg2>(function, pa1, pa2);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename PreArg1, typename PreArg2>
Callback<R(Arg3)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3), PreArg1 pa1,
    PreArg2 pa2) {
  return new FunctionCallback_1_2<false, R, Arg1, Arg2, Arg3, PreArg1,
      PreArg2>(function, pa1, pa2);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4 , typename PreArg1, typename PreArg2, typename PreArg3>
class MemberCallback_1_3 : public Callback<R(Arg4)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4);
 public:
  MemberCallback_1_3(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  ~MemberCallback_1_3() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg4 arg4) {
    CallbackAutoDeleter<MemberCallback_1_3, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, arg4);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename PreArg1,
    typename PreArg2, typename PreArg3>
Callback<R(Arg4)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new MemberCallback_1_3<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename PreArg1,
    typename PreArg2, typename PreArg3>
Callback<R(Arg4)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new MemberCallback_1_3<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4 , typename PreArg1, typename PreArg2, typename PreArg3>
class ConstMemberCallback_1_3 : public Callback<R(Arg4)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4) const;
 public:
  ConstMemberCallback_1_3(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  ~ConstMemberCallback_1_3() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg4 arg4) {
    CallbackAutoDeleter<ConstMemberCallback_1_3, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, arg4);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename PreArg1,
    typename PreArg2, typename PreArg3>
Callback<R(Arg4)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new ConstMemberCallback_1_3<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename PreArg1,
    typename PreArg2, typename PreArg3>
Callback<R(Arg4)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new ConstMemberCallback_1_3<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename PreArg1, typename PreArg2, typename PreArg3>
class FunctionCallback_1_3 : public Callback<R(Arg4)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4);
 public:
  FunctionCallback_1_3(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  FunctionCallback_1_3() {
    function_ = NULL;
  }
  virtual R Run(Arg4 arg4) {
    CallbackAutoDeleter<FunctionCallback_1_3, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, arg4);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4), PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3) {
  return new FunctionCallback_1_3<true, R, Arg1, Arg2, Arg3, Arg4, PreArg1,
      PreArg2, PreArg3>(function, pa1, pa2, pa3);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4), PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3) {
  return new FunctionCallback_1_3<false, R, Arg1, Arg2, Arg3, Arg4, PreArg1,
      PreArg2, PreArg3>(function, pa1, pa2, pa3);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5 , typename PreArg1, typename PreArg2,
      typename PreArg3, typename PreArg4>
class MemberCallback_1_4 : public Callback<R(Arg5)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5);
 public:
  MemberCallback_1_4(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4) {}
  ~MemberCallback_1_4() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg5 arg5) {
    CallbackAutoDeleter<MemberCallback_1_4, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, arg5);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new MemberCallback_1_4<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new MemberCallback_1_4<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5 , typename PreArg1, typename PreArg2,
      typename PreArg3, typename PreArg4>
class ConstMemberCallback_1_4 : public Callback<R(Arg5)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5) const;
 public:
  ConstMemberCallback_1_4(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4) {}
  ~ConstMemberCallback_1_4() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg5 arg5) {
    CallbackAutoDeleter<ConstMemberCallback_1_4, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, arg5);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new ConstMemberCallback_1_4<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4) {
  return new ConstMemberCallback_1_4<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename PreArg1, typename PreArg2, typename PreArg3,
      typename PreArg4>
class FunctionCallback_1_4 : public Callback<R(Arg5)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5);
 public:
  FunctionCallback_1_4(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4) {}
  FunctionCallback_1_4() {
    function_ = NULL;
  }
  virtual R Run(Arg5 arg5) {
    CallbackAutoDeleter<FunctionCallback_1_4, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, arg5);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4>
Callback<R(Arg5)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5), PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new FunctionCallback_1_4<true, R, Arg1, Arg2, Arg3, Arg4, Arg5,
      PreArg1, PreArg2, PreArg3, PreArg4>(function, pa1, pa2, pa3, pa4);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4>
Callback<R(Arg5)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5), PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new FunctionCallback_1_4<false, R, Arg1, Arg2, Arg3, Arg4, Arg5,
      PreArg1, PreArg2, PreArg3, PreArg4>(function, pa1, pa2, pa3, pa4);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6 , typename PreArg1,
      typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5>
class MemberCallback_1_5 : public Callback<R(Arg6)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
 public:
  MemberCallback_1_5(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5) {}
  ~MemberCallback_1_5() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg6 arg6) {
    CallbackAutoDeleter<MemberCallback_1_5, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, arg6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5>
Callback<R(Arg6)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4,
    PreArg5 pa5) {
  return new MemberCallback_1_5<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5>
Callback<R(Arg6)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4,
    PreArg5 pa5) {
  return new MemberCallback_1_5<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6 , typename PreArg1,
      typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5>
class ConstMemberCallback_1_5 : public Callback<R(Arg6)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5,
      Arg6) const;
 public:
  ConstMemberCallback_1_5(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5) {}
  ~ConstMemberCallback_1_5() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg6 arg6) {
    CallbackAutoDeleter<ConstMemberCallback_1_5, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, arg6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5>
Callback<R(Arg6)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5) {
  return new ConstMemberCallback_1_5<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5>
Callback<R(Arg6)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5) {
  return new ConstMemberCallback_1_5<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename PreArg1, typename PreArg2,
      typename PreArg3, typename PreArg4, typename PreArg5>
class FunctionCallback_1_5 : public Callback<R(Arg6)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
 public:
  FunctionCallback_1_5(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4),
          pa_5(pa5) {}
  FunctionCallback_1_5() {
    function_ = NULL;
  }
  virtual R Run(Arg6 arg6) {
    CallbackAutoDeleter<FunctionCallback_1_5, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, pa_5, arg6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5>
Callback<R(Arg6)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new FunctionCallback_1_5<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(function, pa1, pa2, pa3,
      pa4, pa5);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5>
Callback<R(Arg6)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new FunctionCallback_1_5<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(function, pa1, pa2, pa3,
      pa4, pa5);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7 ,
      typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
      typename PreArg5, typename PreArg6>
class MemberCallback_1_6 : public Callback<R(Arg7)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7);
 public:
  MemberCallback_1_6(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5), pa_6(pa6) {}
  ~MemberCallback_1_6() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg7 arg7) {
    CallbackAutoDeleter<MemberCallback_1_6, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6, arg7);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4,
    PreArg5 pa5, PreArg6 pa6) {
  return new MemberCallback_1_6<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5,
      PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new MemberCallback_1_6<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5,
      PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7 ,
      typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
      typename PreArg5, typename PreArg6>
class ConstMemberCallback_1_6 : public Callback<R(Arg7)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7) const;
 public:
  ConstMemberCallback_1_6(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5,
      PreArg6 pa6):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5), pa_6(pa6) {}
  ~ConstMemberCallback_1_6() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg7 arg7) {
    CallbackAutoDeleter<ConstMemberCallback_1_6, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6, arg7);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new ConstMemberCallback_1_6<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new ConstMemberCallback_1_6<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename PreArg1,
      typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5,
      typename PreArg6>
class FunctionCallback_1_6 : public Callback<R(Arg7)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
 public:
  FunctionCallback_1_6(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4),
          pa_5(pa5), pa_6(pa6) {}
  FunctionCallback_1_6() {
    function_ = NULL;
  }
  virtual R Run(Arg7 arg7) {
    CallbackAutoDeleter<FunctionCallback_1_6, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6, arg7);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5, typename PreArg6>
Callback<R(Arg7)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5,
    PreArg6 pa6) {
  return new FunctionCallback_1_6<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5, PreArg6>(function,
      pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5, typename PreArg6>
Callback<R(Arg7)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5,
    PreArg6 pa6) {
  return new FunctionCallback_1_6<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5, PreArg6>(function,
      pa1, pa2, pa3, pa4, pa5, pa6);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2 >
class MemberCallback_2_0 : public Callback<R(Arg1, Arg2)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2);
 public:
  MemberCallback_2_0(Class *object, MemberSignature member):
      object_(object), member_(member) {}
  ~MemberCallback_2_0() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg1 arg1, Arg2 arg2) {
    CallbackAutoDeleter<MemberCallback_2_0, SelfDelete> self_deleter(this);
    return (object_->*member_)(arg1, arg2);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2>
Callback<R(Arg1, Arg2)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2)) {
  return new MemberCallback_2_0<true, R, Class, CallerClass, Arg1, Arg2>(
      object, member);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2>
Callback<R(Arg1, Arg2)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2)) {
  return new MemberCallback_2_0<false, R, Class, CallerClass, Arg1, Arg2>(
      object, member);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2 >
class ConstMemberCallback_2_0 : public Callback<R(Arg1, Arg2)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2) const;
 public:
  ConstMemberCallback_2_0(const Class *object, MemberSignature member):
      object_(object), member_(member) {}
  ~ConstMemberCallback_2_0() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg1 arg1, Arg2 arg2) {
    CallbackAutoDeleter<ConstMemberCallback_2_0, SelfDelete> self_deleter(this);
    return (object_->*member_)(arg1, arg2);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2>
Callback<R(Arg1, Arg2)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2) const) {
  return new ConstMemberCallback_2_0<true, R, Class, CallerClass, Arg1, Arg2>(
      object, member);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2>
Callback<R(Arg1, Arg2)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1,
    Arg2) const) {
  return new ConstMemberCallback_2_0<false, R, Class, CallerClass, Arg1, Arg2>(
      object, member);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2>
class FunctionCallback_2_0 : public Callback<R(Arg1, Arg2)> {
  typedef R (*FunctionType)(Arg1, Arg2);
 public:
  FunctionCallback_2_0(FunctionType function) :
      function_(function) {}
  FunctionCallback_2_0() {
    function_ = NULL;
  }
  virtual R Run(Arg1 arg1, Arg2 arg2) {
    CallbackAutoDeleter<FunctionCallback_2_0, SelfDelete> self_deleter(this);
    return function_(arg1, arg2);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
};

template <typename R, typename Arg1, typename Arg2>
Callback<R(Arg1, Arg2)>*
NewCallback(R (*function)(Arg1, Arg2)) {
  return new FunctionCallback_2_0<true, R, Arg1, Arg2>(function);
}

template <typename R, typename Arg1, typename Arg2>
Callback<R(Arg1, Arg2)>*
NewPermanentCallback(R (*function)(Arg1, Arg2)) {
  return new FunctionCallback_2_0<false, R, Arg1, Arg2>(function);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3 ,
      typename PreArg1>
class MemberCallback_2_1 : public Callback<R(Arg2, Arg3)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3);
 public:
  MemberCallback_2_1(Class *object, MemberSignature member, PreArg1 pa1):
      object_(object), member_(member), pa_1(pa1) {}
  ~MemberCallback_2_1() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg2 arg2, Arg3 arg3) {
    CallbackAutoDeleter<MemberCallback_2_1, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, arg2, arg3);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename PreArg1>
Callback<R(Arg2, Arg3)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3),
    PreArg1 pa1) {
  return new MemberCallback_2_1<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      PreArg1>(
      object, member, pa1);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename PreArg1>
Callback<R(Arg2, Arg3)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3),
    PreArg1 pa1) {
  return new MemberCallback_2_1<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      PreArg1>(
      object, member, pa1);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3 ,
      typename PreArg1>
class ConstMemberCallback_2_1 : public Callback<R(Arg2, Arg3)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3) const;
 public:
  ConstMemberCallback_2_1(const Class *object, MemberSignature member,
      PreArg1 pa1):
      object_(object), member_(member), pa_1(pa1) {}
  ~ConstMemberCallback_2_1() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg2 arg2, Arg3 arg3) {
    CallbackAutoDeleter<ConstMemberCallback_2_1, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, arg2, arg3);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename PreArg1>
Callback<R(Arg2, Arg3)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3) const, PreArg1 pa1) {
  return new ConstMemberCallback_2_1<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, PreArg1>(
      object, member, pa1);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename PreArg1>
Callback<R(Arg2, Arg3)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3) const, PreArg1 pa1) {
  return new ConstMemberCallback_2_1<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, PreArg1>(
      object, member, pa1);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename PreArg1>
class FunctionCallback_2_1 : public Callback<R(Arg2, Arg3)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3);
 public:
  FunctionCallback_2_1(FunctionType function, PreArg1 pa1) :
      function_(function), pa_1(pa1) {}
  FunctionCallback_2_1() {
    function_ = NULL;
  }
  virtual R Run(Arg2 arg2, Arg3 arg3) {
    CallbackAutoDeleter<FunctionCallback_2_1, SelfDelete> self_deleter(this);
    return function_(pa_1, arg2, arg3);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename PreArg1>
Callback<R(Arg2, Arg3)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3), PreArg1 pa1) {
  return new FunctionCallback_2_1<true, R, Arg1, Arg2, Arg3, PreArg1>(function,
      pa1);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename PreArg1>
Callback<R(Arg2, Arg3)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3), PreArg1 pa1) {
  return new FunctionCallback_2_1<false, R, Arg1, Arg2, Arg3,
      PreArg1>(function, pa1);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4 , typename PreArg1, typename PreArg2>
class MemberCallback_2_2 : public Callback<R(Arg3, Arg4)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4);
 public:
  MemberCallback_2_2(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2) {}
  ~MemberCallback_2_2() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg3 arg3, Arg4 arg4) {
    CallbackAutoDeleter<MemberCallback_2_2, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, arg3, arg4);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename PreArg1,
    typename PreArg2>
Callback<R(Arg3, Arg4)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4),
    PreArg1 pa1, PreArg2 pa2) {
  return new MemberCallback_2_2<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename PreArg1,
    typename PreArg2>
Callback<R(Arg3, Arg4)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4), PreArg1 pa1, PreArg2 pa2) {
  return new MemberCallback_2_2<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4 , typename PreArg1, typename PreArg2>
class ConstMemberCallback_2_2 : public Callback<R(Arg3, Arg4)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4) const;
 public:
  ConstMemberCallback_2_2(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2) {}
  ~ConstMemberCallback_2_2() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg3 arg3, Arg4 arg4) {
    CallbackAutoDeleter<ConstMemberCallback_2_2, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, arg3, arg4);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename PreArg1,
    typename PreArg2>
Callback<R(Arg3, Arg4)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4) const, PreArg1 pa1, PreArg2 pa2) {
  return new ConstMemberCallback_2_2<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename PreArg1,
    typename PreArg2>
Callback<R(Arg3, Arg4)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4) const, PreArg1 pa1, PreArg2 pa2) {
  return new ConstMemberCallback_2_2<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename PreArg1, typename PreArg2>
class FunctionCallback_2_2 : public Callback<R(Arg3, Arg4)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4);
 public:
  FunctionCallback_2_2(FunctionType function, PreArg1 pa1, PreArg2 pa2) :
      function_(function), pa_1(pa1), pa_2(pa2) {}
  FunctionCallback_2_2() {
    function_ = NULL;
  }
  virtual R Run(Arg3 arg3, Arg4 arg4) {
    CallbackAutoDeleter<FunctionCallback_2_2, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, arg3, arg4);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4), PreArg1 pa1, PreArg2 pa2) {
  return new FunctionCallback_2_2<true, R, Arg1, Arg2, Arg3, Arg4, PreArg1,
      PreArg2>(function, pa1, pa2);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4), PreArg1 pa1,
    PreArg2 pa2) {
  return new FunctionCallback_2_2<false, R, Arg1, Arg2, Arg3, Arg4, PreArg1,
      PreArg2>(function, pa1, pa2);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5 , typename PreArg1, typename PreArg2,
      typename PreArg3>
class MemberCallback_2_3 : public Callback<R(Arg4, Arg5)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5);
 public:
  MemberCallback_2_3(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  ~MemberCallback_2_3() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg4 arg4, Arg5 arg5) {
    CallbackAutoDeleter<MemberCallback_2_3, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, arg4, arg5);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new MemberCallback_2_3<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new MemberCallback_2_3<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5 , typename PreArg1, typename PreArg2,
      typename PreArg3>
class ConstMemberCallback_2_3 : public Callback<R(Arg4, Arg5)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5) const;
 public:
  ConstMemberCallback_2_3(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  ~ConstMemberCallback_2_3() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg4 arg4, Arg5 arg5) {
    CallbackAutoDeleter<ConstMemberCallback_2_3, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, arg4, arg5);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new ConstMemberCallback_2_3<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new ConstMemberCallback_2_3<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename PreArg1, typename PreArg2, typename PreArg3>
class FunctionCallback_2_3 : public Callback<R(Arg4, Arg5)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5);
 public:
  FunctionCallback_2_3(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  FunctionCallback_2_3() {
    function_ = NULL;
  }
  virtual R Run(Arg4 arg4, Arg5 arg5) {
    CallbackAutoDeleter<FunctionCallback_2_3, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, arg4, arg5);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename PreArg1, typename PreArg2,
    typename PreArg3>
Callback<R(Arg4, Arg5)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5), PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3) {
  return new FunctionCallback_2_3<true, R, Arg1, Arg2, Arg3, Arg4, Arg5,
      PreArg1, PreArg2, PreArg3>(function, pa1, pa2, pa3);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename PreArg1, typename PreArg2,
    typename PreArg3>
Callback<R(Arg4, Arg5)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5), PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3) {
  return new FunctionCallback_2_3<false, R, Arg1, Arg2, Arg3, Arg4, Arg5,
      PreArg1, PreArg2, PreArg3>(function, pa1, pa2, pa3);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6 , typename PreArg1,
      typename PreArg2, typename PreArg3, typename PreArg4>
class MemberCallback_2_4 : public Callback<R(Arg5, Arg6)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
 public:
  MemberCallback_2_4(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4) {}
  ~MemberCallback_2_4() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg5 arg5, Arg6 arg6) {
    CallbackAutoDeleter<MemberCallback_2_4, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, arg5, arg6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new MemberCallback_2_4<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new MemberCallback_2_4<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6 , typename PreArg1,
      typename PreArg2, typename PreArg3, typename PreArg4>
class ConstMemberCallback_2_4 : public Callback<R(Arg5, Arg6)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5,
      Arg6) const;
 public:
  ConstMemberCallback_2_4(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4) {}
  ~ConstMemberCallback_2_4() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg5 arg5, Arg6 arg6) {
    CallbackAutoDeleter<ConstMemberCallback_2_4, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, arg5, arg6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4) {
  return new ConstMemberCallback_2_4<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4) {
  return new ConstMemberCallback_2_4<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename PreArg1, typename PreArg2,
      typename PreArg3, typename PreArg4>
class FunctionCallback_2_4 : public Callback<R(Arg5, Arg6)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
 public:
  FunctionCallback_2_4(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4) {}
  FunctionCallback_2_4() {
    function_ = NULL;
  }
  virtual R Run(Arg5 arg5, Arg6 arg6) {
    CallbackAutoDeleter<FunctionCallback_2_4, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, arg5, arg6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new FunctionCallback_2_4<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      PreArg1, PreArg2, PreArg3, PreArg4>(function, pa1, pa2, pa3, pa4);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new FunctionCallback_2_4<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      PreArg1, PreArg2, PreArg3, PreArg4>(function, pa1, pa2, pa3, pa4);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7 ,
      typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
      typename PreArg5>
class MemberCallback_2_5 : public Callback<R(Arg6, Arg7)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7);
 public:
  MemberCallback_2_5(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5) {}
  ~MemberCallback_2_5() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg6 arg6, Arg7 arg7) {
    CallbackAutoDeleter<MemberCallback_2_5, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, arg6, arg7);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4,
    PreArg5 pa5) {
  return new MemberCallback_2_5<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5) {
  return new MemberCallback_2_5<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7 ,
      typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
      typename PreArg5>
class ConstMemberCallback_2_5 : public Callback<R(Arg6, Arg7)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7) const;
 public:
  ConstMemberCallback_2_5(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5) {}
  ~ConstMemberCallback_2_5() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg6 arg6, Arg7 arg7) {
    CallbackAutoDeleter<ConstMemberCallback_2_5, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, arg6, arg7);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5) {
  return new ConstMemberCallback_2_5<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5) {
  return new ConstMemberCallback_2_5<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename PreArg1,
      typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5>
class FunctionCallback_2_5 : public Callback<R(Arg6, Arg7)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
 public:
  FunctionCallback_2_5(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4),
          pa_5(pa5) {}
  FunctionCallback_2_5() {
    function_ = NULL;
  }
  virtual R Run(Arg6 arg6, Arg7 arg7) {
    CallbackAutoDeleter<FunctionCallback_2_5, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, pa_5, arg6, arg7);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5>
Callback<R(Arg6, Arg7)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new FunctionCallback_2_5<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(function, pa1, pa2,
      pa3, pa4, pa5);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5>
Callback<R(Arg6, Arg7)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new FunctionCallback_2_5<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(function, pa1, pa2,
      pa3, pa4, pa5);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8 , typename PreArg1, typename PreArg2, typename PreArg3,
      typename PreArg4, typename PreArg5, typename PreArg6>
class MemberCallback_2_6 : public Callback<R(Arg7, Arg8)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8);
 public:
  MemberCallback_2_6(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5), pa_6(pa6) {}
  ~MemberCallback_2_6() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg7 arg7, Arg8 arg8) {
    CallbackAutoDeleter<MemberCallback_2_6, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6, arg7, arg8);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7, Arg8), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new MemberCallback_2_6<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new MemberCallback_2_6<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8 , typename PreArg1, typename PreArg2, typename PreArg3,
      typename PreArg4, typename PreArg5, typename PreArg6>
class ConstMemberCallback_2_6 : public Callback<R(Arg7, Arg8)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8) const;
 public:
  ConstMemberCallback_2_6(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5,
      PreArg6 pa6):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5), pa_6(pa6) {}
  ~ConstMemberCallback_2_6() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg7 arg7, Arg8 arg8) {
    CallbackAutoDeleter<ConstMemberCallback_2_6, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6, arg7, arg8);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new ConstMemberCallback_2_6<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) const, PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new ConstMemberCallback_2_6<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename Arg8,
      typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
      typename PreArg5, typename PreArg6>
class FunctionCallback_2_6 : public Callback<R(Arg7, Arg8)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);
 public:
  FunctionCallback_2_6(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4),
          pa_5(pa5), pa_6(pa6) {}
  FunctionCallback_2_6() {
    function_ = NULL;
  }
  virtual R Run(Arg7 arg7, Arg8 arg8) {
    CallbackAutoDeleter<FunctionCallback_2_6, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6, arg7, arg8);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5,
    PreArg6 pa6) {
  return new FunctionCallback_2_6<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5,
      PreArg6>(function, pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,
    Arg8), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5,
    PreArg6 pa6) {
  return new FunctionCallback_2_6<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5,
      PreArg6>(function, pa1, pa2, pa3, pa4, pa5, pa6);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3 >
class MemberCallback_3_0 : public Callback<R(Arg1, Arg2, Arg3)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3);
 public:
  MemberCallback_3_0(Class *object, MemberSignature member):
      object_(object), member_(member) {}
  ~MemberCallback_3_0() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3) {
    CallbackAutoDeleter<MemberCallback_3_0, SelfDelete> self_deleter(this);
    return (object_->*member_)(arg1, arg2, arg3);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3>
Callback<R(Arg1, Arg2, Arg3)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3)) {
  return new MemberCallback_3_0<true, R, Class, CallerClass, Arg1, Arg2, Arg3>(
      object, member);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3>
Callback<R(Arg1, Arg2, Arg3)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3)) {
  return new MemberCallback_3_0<false, R, Class, CallerClass, Arg1, Arg2, Arg3>(
      object, member);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3 >
class ConstMemberCallback_3_0 : public Callback<R(Arg1, Arg2, Arg3)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3) const;
 public:
  ConstMemberCallback_3_0(const Class *object, MemberSignature member):
      object_(object), member_(member) {}
  ~ConstMemberCallback_3_0() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3) {
    CallbackAutoDeleter<ConstMemberCallback_3_0, SelfDelete> self_deleter(this);
    return (object_->*member_)(arg1, arg2, arg3);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3>
Callback<R(Arg1, Arg2, Arg3)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3) const) {
  return new ConstMemberCallback_3_0<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3>(
      object, member);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3>
Callback<R(Arg1, Arg2, Arg3)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3) const) {
  return new ConstMemberCallback_3_0<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3>(
      object, member);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3>
class FunctionCallback_3_0 : public Callback<R(Arg1, Arg2, Arg3)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3);
 public:
  FunctionCallback_3_0(FunctionType function) :
      function_(function) {}
  FunctionCallback_3_0() {
    function_ = NULL;
  }
  virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3) {
    CallbackAutoDeleter<FunctionCallback_3_0, SelfDelete> self_deleter(this);
    return function_(arg1, arg2, arg3);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3>
Callback<R(Arg1, Arg2, Arg3)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3)) {
  return new FunctionCallback_3_0<true, R, Arg1, Arg2, Arg3>(function);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3>
Callback<R(Arg1, Arg2, Arg3)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3)) {
  return new FunctionCallback_3_0<false, R, Arg1, Arg2, Arg3>(function);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4 , typename PreArg1>
class MemberCallback_3_1 : public Callback<R(Arg2, Arg3, Arg4)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4);
 public:
  MemberCallback_3_1(Class *object, MemberSignature member, PreArg1 pa1):
      object_(object), member_(member), pa_1(pa1) {}
  ~MemberCallback_3_1() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg2 arg2, Arg3 arg3, Arg4 arg4) {
    CallbackAutoDeleter<MemberCallback_3_1, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, arg2, arg3, arg4);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename PreArg1>
Callback<R(Arg2, Arg3, Arg4)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4),
    PreArg1 pa1) {
  return new MemberCallback_3_1<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, PreArg1>(
      object, member, pa1);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename PreArg1>
Callback<R(Arg2, Arg3, Arg4)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4), PreArg1 pa1) {
  return new MemberCallback_3_1<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, PreArg1>(
      object, member, pa1);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4 , typename PreArg1>
class ConstMemberCallback_3_1 : public Callback<R(Arg2, Arg3, Arg4)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4) const;
 public:
  ConstMemberCallback_3_1(const Class *object, MemberSignature member,
      PreArg1 pa1):
      object_(object), member_(member), pa_1(pa1) {}
  ~ConstMemberCallback_3_1() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg2 arg2, Arg3 arg3, Arg4 arg4) {
    CallbackAutoDeleter<ConstMemberCallback_3_1, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, arg2, arg3, arg4);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename PreArg1>
Callback<R(Arg2, Arg3, Arg4)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4) const, PreArg1 pa1) {
  return new ConstMemberCallback_3_1<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, PreArg1>(
      object, member, pa1);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename PreArg1>
Callback<R(Arg2, Arg3, Arg4)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4) const, PreArg1 pa1) {
  return new ConstMemberCallback_3_1<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, PreArg1>(
      object, member, pa1);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename PreArg1>
class FunctionCallback_3_1 : public Callback<R(Arg2, Arg3, Arg4)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4);
 public:
  FunctionCallback_3_1(FunctionType function, PreArg1 pa1) :
      function_(function), pa_1(pa1) {}
  FunctionCallback_3_1() {
    function_ = NULL;
  }
  virtual R Run(Arg2 arg2, Arg3 arg3, Arg4 arg4) {
    CallbackAutoDeleter<FunctionCallback_3_1, SelfDelete> self_deleter(this);
    return function_(pa_1, arg2, arg3, arg4);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename PreArg1>
Callback<R(Arg2, Arg3, Arg4)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4), PreArg1 pa1) {
  return new FunctionCallback_3_1<true, R, Arg1, Arg2, Arg3, Arg4,
      PreArg1>(function, pa1);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename PreArg1>
Callback<R(Arg2, Arg3, Arg4)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4), PreArg1 pa1) {
  return new FunctionCallback_3_1<false, R, Arg1, Arg2, Arg3, Arg4,
      PreArg1>(function, pa1);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5 , typename PreArg1, typename PreArg2>
class MemberCallback_3_2 : public Callback<R(Arg3, Arg4, Arg5)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5);
 public:
  MemberCallback_3_2(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2) {}
  ~MemberCallback_3_2() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg3 arg3, Arg4 arg4, Arg5 arg5) {
    CallbackAutoDeleter<MemberCallback_3_2, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, arg3, arg4, arg5);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5), PreArg1 pa1, PreArg2 pa2) {
  return new MemberCallback_3_2<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5), PreArg1 pa1, PreArg2 pa2) {
  return new MemberCallback_3_2<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5 , typename PreArg1, typename PreArg2>
class ConstMemberCallback_3_2 : public Callback<R(Arg3, Arg4, Arg5)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5) const;
 public:
  ConstMemberCallback_3_2(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2) {}
  ~ConstMemberCallback_3_2() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg3 arg3, Arg4 arg4, Arg5 arg5) {
    CallbackAutoDeleter<ConstMemberCallback_3_2, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, arg3, arg4, arg5);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5) const, PreArg1 pa1, PreArg2 pa2) {
  return new ConstMemberCallback_3_2<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5) const, PreArg1 pa1, PreArg2 pa2) {
  return new ConstMemberCallback_3_2<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename PreArg1, typename PreArg2>
class FunctionCallback_3_2 : public Callback<R(Arg3, Arg4, Arg5)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5);
 public:
  FunctionCallback_3_2(FunctionType function, PreArg1 pa1, PreArg2 pa2) :
      function_(function), pa_1(pa1), pa_2(pa2) {}
  FunctionCallback_3_2() {
    function_ = NULL;
  }
  virtual R Run(Arg3 arg3, Arg4 arg4, Arg5 arg5) {
    CallbackAutoDeleter<FunctionCallback_3_2, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, arg3, arg4, arg5);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5), PreArg1 pa1,
    PreArg2 pa2) {
  return new FunctionCallback_3_2<true, R, Arg1, Arg2, Arg3, Arg4, Arg5,
      PreArg1, PreArg2>(function, pa1, pa2);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5), PreArg1 pa1,
    PreArg2 pa2) {
  return new FunctionCallback_3_2<false, R, Arg1, Arg2, Arg3, Arg4, Arg5,
      PreArg1, PreArg2>(function, pa1, pa2);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6 , typename PreArg1,
      typename PreArg2, typename PreArg3>
class MemberCallback_3_3 : public Callback<R(Arg4, Arg5, Arg6)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
 public:
  MemberCallback_3_3(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  ~MemberCallback_3_3() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg4 arg4, Arg5 arg5, Arg6 arg6) {
    CallbackAutoDeleter<MemberCallback_3_3, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, arg4, arg5, arg6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new MemberCallback_3_3<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new MemberCallback_3_3<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6 , typename PreArg1,
      typename PreArg2, typename PreArg3>
class ConstMemberCallback_3_3 : public Callback<R(Arg4, Arg5, Arg6)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5,
      Arg6) const;
 public:
  ConstMemberCallback_3_3(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  ~ConstMemberCallback_3_3() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg4 arg4, Arg5 arg5, Arg6 arg6) {
    CallbackAutoDeleter<ConstMemberCallback_3_3, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, arg4, arg5, arg6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new ConstMemberCallback_3_3<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new ConstMemberCallback_3_3<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename PreArg1, typename PreArg2,
      typename PreArg3>
class FunctionCallback_3_3 : public Callback<R(Arg4, Arg5, Arg6)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
 public:
  FunctionCallback_3_3(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  FunctionCallback_3_3() {
    function_ = NULL;
  }
  virtual R Run(Arg4 arg4, Arg5 arg5, Arg6 arg6) {
    CallbackAutoDeleter<FunctionCallback_3_3, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, arg4, arg5, arg6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename PreArg1,
    typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3) {
  return new FunctionCallback_3_3<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      PreArg1, PreArg2, PreArg3>(function, pa1, pa2, pa3);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename PreArg1,
    typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new FunctionCallback_3_3<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      PreArg1, PreArg2, PreArg3>(function, pa1, pa2, pa3);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7 ,
      typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
class MemberCallback_3_4 : public Callback<R(Arg5, Arg6, Arg7)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7);
 public:
  MemberCallback_3_4(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4) {}
  ~MemberCallback_3_4() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg5 arg5, Arg6 arg6, Arg7 arg7) {
    CallbackAutoDeleter<MemberCallback_3_4, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, arg5, arg6, arg7);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4>
Callback<R(Arg5, Arg6, Arg7)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new MemberCallback_3_4<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4>
Callback<R(Arg5, Arg6, Arg7)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4) {
  return new MemberCallback_3_4<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7 ,
      typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
class ConstMemberCallback_3_4 : public Callback<R(Arg5, Arg6, Arg7)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7) const;
 public:
  ConstMemberCallback_3_4(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4) {}
  ~ConstMemberCallback_3_4() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg5 arg5, Arg6 arg6, Arg7 arg7) {
    CallbackAutoDeleter<ConstMemberCallback_3_4, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, arg5, arg6, arg7);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4>
Callback<R(Arg5, Arg6, Arg7)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4) {
  return new ConstMemberCallback_3_4<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4>
Callback<R(Arg5, Arg6, Arg7)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4) {
  return new ConstMemberCallback_3_4<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename PreArg1,
      typename PreArg2, typename PreArg3, typename PreArg4>
class FunctionCallback_3_4 : public Callback<R(Arg5, Arg6, Arg7)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
 public:
  FunctionCallback_3_4(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4) {}
  FunctionCallback_3_4() {
    function_ = NULL;
  }
  virtual R Run(Arg5 arg5, Arg6 arg6, Arg7 arg7) {
    CallbackAutoDeleter<FunctionCallback_3_4, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, arg5, arg6, arg7);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6, Arg7)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new FunctionCallback_3_4<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, PreArg1, PreArg2, PreArg3, PreArg4>(function, pa1, pa2, pa3, pa4);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6, Arg7)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new FunctionCallback_3_4<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, PreArg1, PreArg2, PreArg3, PreArg4>(function, pa1, pa2, pa3, pa4);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8 , typename PreArg1, typename PreArg2, typename PreArg3,
      typename PreArg4, typename PreArg5>
class MemberCallback_3_5 : public Callback<R(Arg6, Arg7, Arg8)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8);
 public:
  MemberCallback_3_5(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5) {}
  ~MemberCallback_3_5() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg6 arg6, Arg7 arg7, Arg8 arg8) {
    CallbackAutoDeleter<MemberCallback_3_5, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, arg6, arg7, arg8);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7, Arg8)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7, Arg8), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5) {
  return new MemberCallback_3_5<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7, Arg8)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5) {
  return new MemberCallback_3_5<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8 , typename PreArg1, typename PreArg2, typename PreArg3,
      typename PreArg4, typename PreArg5>
class ConstMemberCallback_3_5 : public Callback<R(Arg6, Arg7, Arg8)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8) const;
 public:
  ConstMemberCallback_3_5(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5) {}
  ~ConstMemberCallback_3_5() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg6 arg6, Arg7 arg7, Arg8 arg8) {
    CallbackAutoDeleter<ConstMemberCallback_3_5, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, arg6, arg7, arg8);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7, Arg8)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5) {
  return new ConstMemberCallback_3_5<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7, Arg8)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) const, PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new ConstMemberCallback_3_5<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename Arg8,
      typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
      typename PreArg5>
class FunctionCallback_3_5 : public Callback<R(Arg6, Arg7, Arg8)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);
 public:
  FunctionCallback_3_5(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4),
          pa_5(pa5) {}
  FunctionCallback_3_5() {
    function_ = NULL;
  }
  virtual R Run(Arg6 arg6, Arg7 arg7, Arg8 arg8) {
    CallbackAutoDeleter<FunctionCallback_3_5, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, pa_5, arg6, arg7, arg8);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5>
Callback<R(Arg6, Arg7, Arg8)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new FunctionCallback_3_5<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(function, pa1,
      pa2, pa3, pa4, pa5);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5>
Callback<R(Arg6, Arg7, Arg8)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,
    Arg8), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new FunctionCallback_3_5<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(function, pa1,
      pa2, pa3, pa4, pa5);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9 , typename PreArg1, typename PreArg2,
      typename PreArg3, typename PreArg4, typename PreArg5, typename PreArg6>
class MemberCallback_3_6 : public Callback<R(Arg7, Arg8, Arg9)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9);
 public:
  MemberCallback_3_6(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5), pa_6(pa6) {}
  ~MemberCallback_3_6() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg7 arg7, Arg8 arg8, Arg9 arg9) {
    CallbackAutoDeleter<MemberCallback_3_6, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6, arg7, arg8,
        arg9);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5,
    typename PreArg6>
Callback<R(Arg7, Arg8, Arg9)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7, Arg8, Arg9), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new MemberCallback_3_6<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5,
    typename PreArg6>
Callback<R(Arg7, Arg8, Arg9)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new MemberCallback_3_6<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9 , typename PreArg1, typename PreArg2,
      typename PreArg3, typename PreArg4, typename PreArg5, typename PreArg6>
class ConstMemberCallback_3_6 : public Callback<R(Arg7, Arg8, Arg9)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9) const;
 public:
  ConstMemberCallback_3_6(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5,
      PreArg6 pa6):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5), pa_6(pa6) {}
  ~ConstMemberCallback_3_6() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg7 arg7, Arg8 arg8, Arg9 arg9) {
    CallbackAutoDeleter<ConstMemberCallback_3_6, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6, arg7, arg8,
        arg9);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5,
    typename PreArg6>
Callback<R(Arg7, Arg8, Arg9)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) const, PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new ConstMemberCallback_3_6<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3,
      PreArg4, PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5,
    typename PreArg6>
Callback<R(Arg7, Arg8, Arg9)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) const, PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new ConstMemberCallback_3_6<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3,
      PreArg4, PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename Arg8,
      typename Arg9, typename PreArg1, typename PreArg2, typename PreArg3,
      typename PreArg4, typename PreArg5, typename PreArg6>
class FunctionCallback_3_6 : public Callback<R(Arg7, Arg8, Arg9)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8,
      Arg9);
 public:
  FunctionCallback_3_6(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4),
          pa_5(pa5), pa_6(pa6) {}
  FunctionCallback_3_6() {
    function_ = NULL;
  }
  virtual R Run(Arg7 arg7, Arg8 arg8, Arg9 arg9) {
    CallbackAutoDeleter<FunctionCallback_3_6, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6, arg7, arg8, arg9);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8, Arg9)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8,
    Arg9), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5,
    PreArg6 pa6) {
  return new FunctionCallback_3_6<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5,
      PreArg6>(function, pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8, Arg9)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,
    Arg8, Arg9), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4,
    PreArg5 pa5, PreArg6 pa6) {
  return new FunctionCallback_3_6<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5,
      PreArg6>(function, pa1, pa2, pa3, pa4, pa5, pa6);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4 >
class MemberCallback_4_0 : public Callback<R(Arg1, Arg2, Arg3, Arg4)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4);
 public:
  MemberCallback_4_0(Class *object, MemberSignature member):
      object_(object), member_(member) {}
  ~MemberCallback_4_0() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) {
    CallbackAutoDeleter<MemberCallback_4_0, SelfDelete> self_deleter(this);
    return (object_->*member_)(arg1, arg2, arg3, arg4);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4>
Callback<R(Arg1, Arg2, Arg3, Arg4)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4)) {
  return new MemberCallback_4_0<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4>(
      object, member);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4>
Callback<R(Arg1, Arg2, Arg3, Arg4)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4)) {
  return new MemberCallback_4_0<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4>(
      object, member);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4 >
class ConstMemberCallback_4_0 : public Callback<R(Arg1, Arg2, Arg3, Arg4)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4) const;
 public:
  ConstMemberCallback_4_0(const Class *object, MemberSignature member):
      object_(object), member_(member) {}
  ~ConstMemberCallback_4_0() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) {
    CallbackAutoDeleter<ConstMemberCallback_4_0, SelfDelete> self_deleter(this);
    return (object_->*member_)(arg1, arg2, arg3, arg4);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4>
Callback<R(Arg1, Arg2, Arg3, Arg4)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4) const) {
  return new ConstMemberCallback_4_0<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4>(
      object, member);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4>
Callback<R(Arg1, Arg2, Arg3, Arg4)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4) const) {
  return new ConstMemberCallback_4_0<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4>(
      object, member);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
class FunctionCallback_4_0 : public Callback<R(Arg1, Arg2, Arg3, Arg4)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4);
 public:
  FunctionCallback_4_0(FunctionType function) :
      function_(function) {}
  FunctionCallback_4_0() {
    function_ = NULL;
  }
  virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) {
    CallbackAutoDeleter<FunctionCallback_4_0, SelfDelete> self_deleter(this);
    return function_(arg1, arg2, arg3, arg4);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4>
Callback<R(Arg1, Arg2, Arg3, Arg4)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4)) {
  return new FunctionCallback_4_0<true, R, Arg1, Arg2, Arg3, Arg4>(function);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4>
Callback<R(Arg1, Arg2, Arg3, Arg4)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4)) {
  return new FunctionCallback_4_0<false, R, Arg1, Arg2, Arg3, Arg4>(function);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5 , typename PreArg1>
class MemberCallback_4_1 : public Callback<R(Arg2, Arg3, Arg4, Arg5)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5);
 public:
  MemberCallback_4_1(Class *object, MemberSignature member, PreArg1 pa1):
      object_(object), member_(member), pa_1(pa1) {}
  ~MemberCallback_4_1() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) {
    CallbackAutoDeleter<MemberCallback_4_1, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, arg2, arg3, arg4, arg5);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1>
Callback<R(Arg2, Arg3, Arg4, Arg5)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5), PreArg1 pa1) {
  return new MemberCallback_4_1<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, PreArg1>(
      object, member, pa1);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1>
Callback<R(Arg2, Arg3, Arg4, Arg5)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5), PreArg1 pa1) {
  return new MemberCallback_4_1<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, PreArg1>(
      object, member, pa1);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5 , typename PreArg1>
class ConstMemberCallback_4_1 : public Callback<R(Arg2, Arg3, Arg4, Arg5)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5) const;
 public:
  ConstMemberCallback_4_1(const Class *object, MemberSignature member,
      PreArg1 pa1):
      object_(object), member_(member), pa_1(pa1) {}
  ~ConstMemberCallback_4_1() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) {
    CallbackAutoDeleter<ConstMemberCallback_4_1, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, arg2, arg3, arg4, arg5);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1>
Callback<R(Arg2, Arg3, Arg4, Arg5)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5) const, PreArg1 pa1) {
  return new ConstMemberCallback_4_1<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, PreArg1>(
      object, member, pa1);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5,
    typename PreArg1>
Callback<R(Arg2, Arg3, Arg4, Arg5)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5) const, PreArg1 pa1) {
  return new ConstMemberCallback_4_1<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, PreArg1>(
      object, member, pa1);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename PreArg1>
class FunctionCallback_4_1 : public Callback<R(Arg2, Arg3, Arg4, Arg5)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5);
 public:
  FunctionCallback_4_1(FunctionType function, PreArg1 pa1) :
      function_(function), pa_1(pa1) {}
  FunctionCallback_4_1() {
    function_ = NULL;
  }
  virtual R Run(Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) {
    CallbackAutoDeleter<FunctionCallback_4_1, SelfDelete> self_deleter(this);
    return function_(pa_1, arg2, arg3, arg4, arg5);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename PreArg1>
Callback<R(Arg2, Arg3, Arg4, Arg5)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5), PreArg1 pa1) {
  return new FunctionCallback_4_1<true, R, Arg1, Arg2, Arg3, Arg4, Arg5,
      PreArg1>(function, pa1);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename PreArg1>
Callback<R(Arg2, Arg3, Arg4, Arg5)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5), PreArg1 pa1) {
  return new FunctionCallback_4_1<false, R, Arg1, Arg2, Arg3, Arg4, Arg5,
      PreArg1>(function, pa1);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6 , typename PreArg1,
      typename PreArg2>
class MemberCallback_4_2 : public Callback<R(Arg3, Arg4, Arg5, Arg6)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
 public:
  MemberCallback_4_2(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2) {}
  ~MemberCallback_4_2() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6) {
    CallbackAutoDeleter<MemberCallback_4_2, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, arg3, arg4, arg5, arg6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5, Arg6)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6), PreArg1 pa1, PreArg2 pa2) {
  return new MemberCallback_4_2<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5, Arg6)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6), PreArg1 pa1, PreArg2 pa2) {
  return new MemberCallback_4_2<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6 , typename PreArg1,
      typename PreArg2>
class ConstMemberCallback_4_2 : public Callback<R(Arg3, Arg4, Arg5, Arg6)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5,
      Arg6) const;
 public:
  ConstMemberCallback_4_2(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2) {}
  ~ConstMemberCallback_4_2() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6) {
    CallbackAutoDeleter<ConstMemberCallback_4_2, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, arg3, arg4, arg5, arg6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5, Arg6)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6) const, PreArg1 pa1, PreArg2 pa2) {
  return new ConstMemberCallback_4_2<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5, Arg6)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6) const, PreArg1 pa1, PreArg2 pa2) {
  return new ConstMemberCallback_4_2<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename PreArg1, typename PreArg2>
class FunctionCallback_4_2 : public Callback<R(Arg3, Arg4, Arg5, Arg6)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
 public:
  FunctionCallback_4_2(FunctionType function, PreArg1 pa1, PreArg2 pa2) :
      function_(function), pa_1(pa1), pa_2(pa2) {}
  FunctionCallback_4_2() {
    function_ = NULL;
  }
  virtual R Run(Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6) {
    CallbackAutoDeleter<FunctionCallback_4_2, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, arg3, arg4, arg5, arg6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename PreArg1,
    typename PreArg2>
Callback<R(Arg3, Arg4, Arg5, Arg6)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), PreArg1 pa1,
    PreArg2 pa2) {
  return new FunctionCallback_4_2<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      PreArg1, PreArg2>(function, pa1, pa2);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename PreArg1,
    typename PreArg2>
Callback<R(Arg3, Arg4, Arg5, Arg6)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6),
    PreArg1 pa1, PreArg2 pa2) {
  return new FunctionCallback_4_2<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      PreArg1, PreArg2>(function, pa1, pa2);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7 ,
      typename PreArg1, typename PreArg2, typename PreArg3>
class MemberCallback_4_3 : public Callback<R(Arg4, Arg5, Arg6, Arg7)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7);
 public:
  MemberCallback_4_3(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  ~MemberCallback_4_3() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7) {
    CallbackAutoDeleter<MemberCallback_4_3, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, arg4, arg5, arg6, arg7);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6, Arg7)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new MemberCallback_4_3<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6, Arg7)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new MemberCallback_4_3<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7 ,
      typename PreArg1, typename PreArg2, typename PreArg3>
class ConstMemberCallback_4_3 : public Callback<R(Arg4, Arg5, Arg6, Arg7)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7) const;
 public:
  ConstMemberCallback_4_3(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  ~ConstMemberCallback_4_3() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7) {
    CallbackAutoDeleter<ConstMemberCallback_4_3, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, arg4, arg5, arg6, arg7);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6, Arg7)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new ConstMemberCallback_4_3<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6, Arg7)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7) const, PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3) {
  return new ConstMemberCallback_4_3<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename PreArg1,
      typename PreArg2, typename PreArg3>
class FunctionCallback_4_3 : public Callback<R(Arg4, Arg5, Arg6, Arg7)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
 public:
  FunctionCallback_4_3(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  FunctionCallback_4_3() {
    function_ = NULL;
  }
  virtual R Run(Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7) {
    CallbackAutoDeleter<FunctionCallback_4_3, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, arg4, arg5, arg6, arg7);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7,
    typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6, Arg7)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new FunctionCallback_4_3<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, PreArg1, PreArg2, PreArg3>(function, pa1, pa2, pa3);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7,
    typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6, Arg7)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new FunctionCallback_4_3<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, PreArg1, PreArg2, PreArg3>(function, pa1, pa2, pa3);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8 , typename PreArg1, typename PreArg2, typename PreArg3,
      typename PreArg4>
class MemberCallback_4_4 : public Callback<R(Arg5, Arg6, Arg7, Arg8)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8);
 public:
  MemberCallback_4_4(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4) {}
  ~MemberCallback_4_4() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8) {
    CallbackAutoDeleter<MemberCallback_4_4, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, arg5, arg6, arg7, arg8);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6, Arg7, Arg8)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7, Arg8), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4) {
  return new MemberCallback_4_4<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6, Arg7, Arg8)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4) {
  return new MemberCallback_4_4<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8 , typename PreArg1, typename PreArg2, typename PreArg3,
      typename PreArg4>
class ConstMemberCallback_4_4 : public Callback<R(Arg5, Arg6, Arg7, Arg8)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8) const;
 public:
  ConstMemberCallback_4_4(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4) {}
  ~ConstMemberCallback_4_4() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8) {
    CallbackAutoDeleter<ConstMemberCallback_4_4, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, arg5, arg6, arg7, arg8);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6, Arg7, Arg8)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8) const, PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4) {
  return new ConstMemberCallback_4_4<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6, Arg7, Arg8)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) const, PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4) {
  return new ConstMemberCallback_4_4<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename Arg8,
      typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
class FunctionCallback_4_4 : public Callback<R(Arg5, Arg6, Arg7, Arg8)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);
 public:
  FunctionCallback_4_4(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4) {}
  FunctionCallback_4_4() {
    function_ = NULL;
  }
  virtual R Run(Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8) {
    CallbackAutoDeleter<FunctionCallback_4_4, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, arg5, arg6, arg7, arg8);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6, Arg7, Arg8)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new FunctionCallback_4_4<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, PreArg1, PreArg2, PreArg3, PreArg4>(function, pa1, pa2, pa3,
      pa4);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6, Arg7, Arg8)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,
    Arg8), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new FunctionCallback_4_4<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, PreArg1, PreArg2, PreArg3, PreArg4>(function, pa1, pa2, pa3,
      pa4);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9 , typename PreArg1, typename PreArg2,
      typename PreArg3, typename PreArg4, typename PreArg5>
class MemberCallback_4_5 : public Callback<R(Arg6, Arg7, Arg8, Arg9)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9);
 public:
  MemberCallback_4_5(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5) {}
  ~MemberCallback_4_5() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9) {
    CallbackAutoDeleter<MemberCallback_4_5, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, arg6, arg7, arg8,
        arg9);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7, Arg8, Arg9)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7, Arg8, Arg9), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5) {
  return new MemberCallback_4_5<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7, Arg8, Arg9)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5) {
  return new MemberCallback_4_5<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9 , typename PreArg1, typename PreArg2,
      typename PreArg3, typename PreArg4, typename PreArg5>
class ConstMemberCallback_4_5 : public Callback<R(Arg6, Arg7, Arg8, Arg9)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9) const;
 public:
  ConstMemberCallback_4_5(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5) {}
  ~ConstMemberCallback_4_5() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9) {
    CallbackAutoDeleter<ConstMemberCallback_4_5, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, arg6, arg7, arg8,
        arg9);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7, Arg8, Arg9)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) const, PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new ConstMemberCallback_4_5<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3,
      PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7, Arg8, Arg9)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) const, PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new ConstMemberCallback_4_5<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3,
      PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename Arg8,
      typename Arg9, typename PreArg1, typename PreArg2, typename PreArg3,
      typename PreArg4, typename PreArg5>
class FunctionCallback_4_5 : public Callback<R(Arg6, Arg7, Arg8, Arg9)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8,
      Arg9);
 public:
  FunctionCallback_4_5(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4),
          pa_5(pa5) {}
  FunctionCallback_4_5() {
    function_ = NULL;
  }
  virtual R Run(Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9) {
    CallbackAutoDeleter<FunctionCallback_4_5, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, pa_5, arg6, arg7, arg8, arg9);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7, Arg8, Arg9)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8,
    Arg9), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new FunctionCallback_4_5<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(function,
      pa1, pa2, pa3, pa4, pa5);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7, Arg8, Arg9)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,
    Arg8, Arg9), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4,
    PreArg5 pa5) {
  return new FunctionCallback_4_5<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5>(function,
      pa1, pa2, pa3, pa4, pa5);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9, typename Arg10 , typename PreArg1,
      typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5,
      typename PreArg6>
class MemberCallback_4_6 : public Callback<R(Arg7, Arg8, Arg9, Arg10)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10);
 public:
  MemberCallback_4_6(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5), pa_6(pa6) {}
  ~MemberCallback_4_6() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10) {
    CallbackAutoDeleter<MemberCallback_4_6, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6, arg7, arg8,
        arg9, arg10);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8, Arg9, Arg10)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new MemberCallback_4_6<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, PreArg1, PreArg2, PreArg3,
      PreArg4, PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8, Arg9, Arg10)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new MemberCallback_4_6<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, PreArg1, PreArg2, PreArg3,
      PreArg4, PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9, typename Arg10 , typename PreArg1,
      typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5,
      typename PreArg6>
class ConstMemberCallback_4_6 : public Callback<R(Arg7, Arg8, Arg9, Arg10)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10) const;
 public:
  ConstMemberCallback_4_6(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5,
      PreArg6 pa6):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5), pa_6(pa6) {}
  ~ConstMemberCallback_4_6() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10) {
    CallbackAutoDeleter<ConstMemberCallback_4_6, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6, arg7, arg8,
        arg9, arg10);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8, Arg9, Arg10)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) const, PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new ConstMemberCallback_4_6<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, PreArg1, PreArg2,
      PreArg3, PreArg4, PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8, Arg9, Arg10)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) const, PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new ConstMemberCallback_4_6<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, PreArg1, PreArg2,
      PreArg3, PreArg4, PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename Arg8,
      typename Arg9, typename Arg10, typename PreArg1, typename PreArg2,
      typename PreArg3, typename PreArg4, typename PreArg5, typename PreArg6>
class FunctionCallback_4_6 : public Callback<R(Arg7, Arg8, Arg9, Arg10)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8,
      Arg9, Arg10);
 public:
  FunctionCallback_4_6(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4),
          pa_5(pa5), pa_6(pa6) {}
  FunctionCallback_4_6() {
    function_ = NULL;
  }
  virtual R Run(Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10) {
    CallbackAutoDeleter<FunctionCallback_4_6, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6, arg7, arg8, arg9,
        arg10);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename Arg10, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8, Arg9, Arg10)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9,
    Arg10), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5,
    PreArg6 pa6) {
  return new FunctionCallback_4_6<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5,
      PreArg6>(function, pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename Arg10, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8, Arg9, Arg10)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,
    Arg8, Arg9, Arg10), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4,
    PreArg5 pa5, PreArg6 pa6) {
  return new FunctionCallback_4_6<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10, PreArg1, PreArg2, PreArg3, PreArg4, PreArg5,
      PreArg6>(function, pa1, pa2, pa3, pa4, pa5, pa6);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5 >
class MemberCallback_5_0 : public Callback<R(Arg1, Arg2, Arg3, Arg4, Arg5)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5);
 public:
  MemberCallback_5_0(Class *object, MemberSignature member):
      object_(object), member_(member) {}
  ~MemberCallback_5_0() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) {
    CallbackAutoDeleter<MemberCallback_5_0, SelfDelete> self_deleter(this);
    return (object_->*member_)(arg1, arg2, arg3, arg4, arg5);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5>
Callback<R(Arg1, Arg2, Arg3, Arg4, Arg5)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5)) {
  return new MemberCallback_5_0<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5>(
      object, member);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5>
Callback<R(Arg1, Arg2, Arg3, Arg4, Arg5)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5)) {
  return new MemberCallback_5_0<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5>(
      object, member);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5 >
class ConstMemberCallback_5_0 : public Callback<R(Arg1, Arg2, Arg3, Arg4,
    Arg5)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5) const;
 public:
  ConstMemberCallback_5_0(const Class *object, MemberSignature member):
      object_(object), member_(member) {}
  ~ConstMemberCallback_5_0() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) {
    CallbackAutoDeleter<ConstMemberCallback_5_0, SelfDelete> self_deleter(this);
    return (object_->*member_)(arg1, arg2, arg3, arg4, arg5);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5>
Callback<R(Arg1, Arg2, Arg3, Arg4, Arg5)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5) const) {
  return new ConstMemberCallback_5_0<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5>(
      object, member);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5>
Callback<R(Arg1, Arg2, Arg3, Arg4, Arg5)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5) const) {
  return new ConstMemberCallback_5_0<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5>(
      object, member);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5>
class FunctionCallback_5_0 : public Callback<R(Arg1, Arg2, Arg3, Arg4, Arg5)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5);
 public:
  FunctionCallback_5_0(FunctionType function) :
      function_(function) {}
  FunctionCallback_5_0() {
    function_ = NULL;
  }
  virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) {
    CallbackAutoDeleter<FunctionCallback_5_0, SelfDelete> self_deleter(this);
    return function_(arg1, arg2, arg3, arg4, arg5);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5>
Callback<R(Arg1, Arg2, Arg3, Arg4, Arg5)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5)) {
  return new FunctionCallback_5_0<true, R, Arg1, Arg2, Arg3, Arg4,
      Arg5>(function);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5>
Callback<R(Arg1, Arg2, Arg3, Arg4, Arg5)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5)) {
  return new FunctionCallback_5_0<false, R, Arg1, Arg2, Arg3, Arg4,
      Arg5>(function);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6 , typename PreArg1>
class MemberCallback_5_1 : public Callback<R(Arg2, Arg3, Arg4, Arg5, Arg6)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
 public:
  MemberCallback_5_1(Class *object, MemberSignature member, PreArg1 pa1):
      object_(object), member_(member), pa_1(pa1) {}
  ~MemberCallback_5_1() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6) {
    CallbackAutoDeleter<MemberCallback_5_1, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, arg2, arg3, arg4, arg5, arg6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1>
Callback<R(Arg2, Arg3, Arg4, Arg5, Arg6)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6), PreArg1 pa1) {
  return new MemberCallback_5_1<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, PreArg1>(
      object, member, pa1);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1>
Callback<R(Arg2, Arg3, Arg4, Arg5, Arg6)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6), PreArg1 pa1) {
  return new MemberCallback_5_1<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, PreArg1>(
      object, member, pa1);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6 , typename PreArg1>
class ConstMemberCallback_5_1 : public Callback<R(Arg2, Arg3, Arg4, Arg5,
    Arg6)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5,
      Arg6) const;
 public:
  ConstMemberCallback_5_1(const Class *object, MemberSignature member,
      PreArg1 pa1):
      object_(object), member_(member), pa_1(pa1) {}
  ~ConstMemberCallback_5_1() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6) {
    CallbackAutoDeleter<ConstMemberCallback_5_1, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, arg2, arg3, arg4, arg5, arg6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1>
Callback<R(Arg2, Arg3, Arg4, Arg5, Arg6)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6) const, PreArg1 pa1) {
  return new ConstMemberCallback_5_1<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, PreArg1>(
      object, member, pa1);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename PreArg1>
Callback<R(Arg2, Arg3, Arg4, Arg5, Arg6)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6) const, PreArg1 pa1) {
  return new ConstMemberCallback_5_1<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, PreArg1>(
      object, member, pa1);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename PreArg1>
class FunctionCallback_5_1 : public Callback<R(Arg2, Arg3, Arg4, Arg5, Arg6)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
 public:
  FunctionCallback_5_1(FunctionType function, PreArg1 pa1) :
      function_(function), pa_1(pa1) {}
  FunctionCallback_5_1() {
    function_ = NULL;
  }
  virtual R Run(Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6) {
    CallbackAutoDeleter<FunctionCallback_5_1, SelfDelete> self_deleter(this);
    return function_(pa_1, arg2, arg3, arg4, arg5, arg6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename PreArg1>
Callback<R(Arg2, Arg3, Arg4, Arg5, Arg6)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6), PreArg1 pa1) {
  return new FunctionCallback_5_1<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      PreArg1>(function, pa1);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename PreArg1>
Callback<R(Arg2, Arg3, Arg4, Arg5, Arg6)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6),
    PreArg1 pa1) {
  return new FunctionCallback_5_1<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      PreArg1>(function, pa1);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7 ,
      typename PreArg1, typename PreArg2>
class MemberCallback_5_2 : public Callback<R(Arg3, Arg4, Arg5, Arg6, Arg7)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7);
 public:
  MemberCallback_5_2(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2) {}
  ~MemberCallback_5_2() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7) {
    CallbackAutoDeleter<MemberCallback_5_2, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, arg3, arg4, arg5, arg6, arg7);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5, Arg6, Arg7)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7), PreArg1 pa1, PreArg2 pa2) {
  return new MemberCallback_5_2<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5, Arg6, Arg7)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7), PreArg1 pa1, PreArg2 pa2) {
  return new MemberCallback_5_2<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7 ,
      typename PreArg1, typename PreArg2>
class ConstMemberCallback_5_2 : public Callback<R(Arg3, Arg4, Arg5, Arg6,
    Arg7)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7) const;
 public:
  ConstMemberCallback_5_2(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2) {}
  ~ConstMemberCallback_5_2() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7) {
    CallbackAutoDeleter<ConstMemberCallback_5_2, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, arg3, arg4, arg5, arg6, arg7);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5, Arg6, Arg7)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7) const, PreArg1 pa1, PreArg2 pa2) {
  return new ConstMemberCallback_5_2<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5, Arg6, Arg7)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7) const, PreArg1 pa1, PreArg2 pa2) {
  return new ConstMemberCallback_5_2<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename PreArg1,
      typename PreArg2>
class FunctionCallback_5_2 : public Callback<R(Arg3, Arg4, Arg5, Arg6, Arg7)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
 public:
  FunctionCallback_5_2(FunctionType function, PreArg1 pa1, PreArg2 pa2) :
      function_(function), pa_1(pa1), pa_2(pa2) {}
  FunctionCallback_5_2() {
    function_ = NULL;
  }
  virtual R Run(Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7) {
    CallbackAutoDeleter<FunctionCallback_5_2, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, arg3, arg4, arg5, arg6, arg7);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7,
    typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5, Arg6, Arg7)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7),
    PreArg1 pa1, PreArg2 pa2) {
  return new FunctionCallback_5_2<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, PreArg1, PreArg2>(function, pa1, pa2);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7,
    typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5, Arg6, Arg7)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7),
    PreArg1 pa1, PreArg2 pa2) {
  return new FunctionCallback_5_2<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, PreArg1, PreArg2>(function, pa1, pa2);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8 , typename PreArg1, typename PreArg2, typename PreArg3>
class MemberCallback_5_3 : public Callback<R(Arg4, Arg5, Arg6, Arg7, Arg8)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8);
 public:
  MemberCallback_5_3(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  ~MemberCallback_5_3() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8) {
    CallbackAutoDeleter<MemberCallback_5_3, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, arg4, arg5, arg6, arg7, arg8);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2,
    typename PreArg3>
Callback<R(Arg4, Arg5, Arg6, Arg7, Arg8)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7, Arg8), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new MemberCallback_5_3<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2,
    typename PreArg3>
Callback<R(Arg4, Arg5, Arg6, Arg7, Arg8)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new MemberCallback_5_3<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8 , typename PreArg1, typename PreArg2, typename PreArg3>
class ConstMemberCallback_5_3 : public Callback<R(Arg4, Arg5, Arg6, Arg7,
    Arg8)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8) const;
 public:
  ConstMemberCallback_5_3(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  ~ConstMemberCallback_5_3() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8) {
    CallbackAutoDeleter<ConstMemberCallback_5_3, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, arg4, arg5, arg6, arg7, arg8);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2,
    typename PreArg3>
Callback<R(Arg4, Arg5, Arg6, Arg7, Arg8)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8) const, PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3) {
  return new ConstMemberCallback_5_3<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2,
    typename PreArg3>
Callback<R(Arg4, Arg5, Arg6, Arg7, Arg8)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) const, PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3) {
  return new ConstMemberCallback_5_3<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename Arg8,
      typename PreArg1, typename PreArg2, typename PreArg3>
class FunctionCallback_5_3 : public Callback<R(Arg4, Arg5, Arg6, Arg7, Arg8)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);
 public:
  FunctionCallback_5_3(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  FunctionCallback_5_3() {
    function_ = NULL;
  }
  virtual R Run(Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8) {
    CallbackAutoDeleter<FunctionCallback_5_3, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, arg4, arg5, arg6, arg7, arg8);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6, Arg7, Arg8)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8),
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new FunctionCallback_5_3<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, PreArg1, PreArg2, PreArg3>(function, pa1, pa2, pa3);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6, Arg7, Arg8)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,
    Arg8), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new FunctionCallback_5_3<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, PreArg1, PreArg2, PreArg3>(function, pa1, pa2, pa3);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9 , typename PreArg1, typename PreArg2,
      typename PreArg3, typename PreArg4>
class MemberCallback_5_4 : public Callback<R(Arg5, Arg6, Arg7, Arg8, Arg9)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9);
 public:
  MemberCallback_5_4(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4) {}
  ~MemberCallback_5_4() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9) {
    CallbackAutoDeleter<MemberCallback_5_4, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, arg5, arg6, arg7, arg8,
        arg9);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6, Arg7, Arg8, Arg9)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7, Arg8, Arg9), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4) {
  return new MemberCallback_5_4<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6, Arg7, Arg8, Arg9)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4) {
  return new MemberCallback_5_4<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9 , typename PreArg1, typename PreArg2,
      typename PreArg3, typename PreArg4>
class ConstMemberCallback_5_4 : public Callback<R(Arg5, Arg6, Arg7, Arg8,
    Arg9)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9) const;
 public:
  ConstMemberCallback_5_4(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4) {}
  ~ConstMemberCallback_5_4() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9) {
    CallbackAutoDeleter<ConstMemberCallback_5_4, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, arg5, arg6, arg7, arg8,
        arg9);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6, Arg7, Arg8, Arg9)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) const, PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4) {
  return new ConstMemberCallback_5_4<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3,
      PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6, Arg7, Arg8, Arg9)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) const, PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4) {
  return new ConstMemberCallback_5_4<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3,
      PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename Arg8,
      typename Arg9, typename PreArg1, typename PreArg2, typename PreArg3,
      typename PreArg4>
class FunctionCallback_5_4 : public Callback<R(Arg5, Arg6, Arg7, Arg8, Arg9)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8,
      Arg9);
 public:
  FunctionCallback_5_4(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4) {}
  FunctionCallback_5_4() {
    function_ = NULL;
  }
  virtual R Run(Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9) {
    CallbackAutoDeleter<FunctionCallback_5_4, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, arg5, arg6, arg7, arg8, arg9);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4>
Callback<R(Arg5, Arg6, Arg7, Arg8, Arg9)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8,
    Arg9), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new FunctionCallback_5_4<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3, PreArg4>(function, pa1, pa2,
      pa3, pa4);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4>
Callback<R(Arg5, Arg6, Arg7, Arg8, Arg9)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,
    Arg8, Arg9), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new FunctionCallback_5_4<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3, PreArg4>(function, pa1, pa2,
      pa3, pa4);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9, typename Arg10 , typename PreArg1,
      typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5>
class MemberCallback_5_5 : public Callback<R(Arg6, Arg7, Arg8, Arg9, Arg10)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10);
 public:
  MemberCallback_5_5(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5) {}
  ~MemberCallback_5_5() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10) {
    CallbackAutoDeleter<MemberCallback_5_5, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, arg6, arg7, arg8,
        arg9, arg10);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5>
Callback<R(Arg6, Arg7, Arg8, Arg9, Arg10)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new MemberCallback_5_5<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, PreArg1, PreArg2, PreArg3,
      PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5>
Callback<R(Arg6, Arg7, Arg8, Arg9, Arg10)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new MemberCallback_5_5<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, PreArg1, PreArg2, PreArg3,
      PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9, typename Arg10 , typename PreArg1,
      typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5>
class ConstMemberCallback_5_5 : public Callback<R(Arg6, Arg7, Arg8, Arg9,
    Arg10)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10) const;
 public:
  ConstMemberCallback_5_5(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5) {}
  ~ConstMemberCallback_5_5() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10) {
    CallbackAutoDeleter<ConstMemberCallback_5_5, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, arg6, arg7, arg8,
        arg9, arg10);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5>
Callback<R(Arg6, Arg7, Arg8, Arg9, Arg10)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) const, PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new ConstMemberCallback_5_5<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, PreArg1, PreArg2,
      PreArg3, PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5>
Callback<R(Arg6, Arg7, Arg8, Arg9, Arg10)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) const, PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new ConstMemberCallback_5_5<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, PreArg1, PreArg2,
      PreArg3, PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename Arg8,
      typename Arg9, typename Arg10, typename PreArg1, typename PreArg2,
      typename PreArg3, typename PreArg4, typename PreArg5>
class FunctionCallback_5_5 : public Callback<R(Arg6, Arg7, Arg8, Arg9, Arg10)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8,
      Arg9, Arg10);
 public:
  FunctionCallback_5_5(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4),
          pa_5(pa5) {}
  FunctionCallback_5_5() {
    function_ = NULL;
  }
  virtual R Run(Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10) {
    CallbackAutoDeleter<FunctionCallback_5_5, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, pa_5, arg6, arg7, arg8, arg9,
        arg10);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename Arg10, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7, Arg8, Arg9, Arg10)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9,
    Arg10), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new FunctionCallback_5_5<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5>(function, pa1, pa2, pa3, pa4, pa5);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename Arg10, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7, Arg8, Arg9, Arg10)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,
    Arg8, Arg9, Arg10), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4,
    PreArg5 pa5) {
  return new FunctionCallback_5_5<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5>(function, pa1, pa2, pa3, pa4, pa5);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9, typename Arg10, typename Arg11 ,
      typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
      typename PreArg5, typename PreArg6>
class MemberCallback_5_6 : public Callback<R(Arg7, Arg8, Arg9, Arg10, Arg11)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10, Arg11);
 public:
  MemberCallback_5_6(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5), pa_6(pa6) {}
  ~MemberCallback_5_6() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10, Arg11 arg11) {
    CallbackAutoDeleter<MemberCallback_5_6, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6, arg7, arg8,
        arg9, arg10, arg11);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename Arg11, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8, Arg9, Arg10, Arg11)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11), PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new MemberCallback_5_6<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, PreArg1, PreArg2,
      PreArg3, PreArg4, PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename Arg11, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8, Arg9, Arg10, Arg11)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11), PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new MemberCallback_5_6<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, PreArg1, PreArg2,
      PreArg3, PreArg4, PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9, typename Arg10, typename Arg11 ,
      typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
      typename PreArg5, typename PreArg6>
class ConstMemberCallback_5_6 : public Callback<R(Arg7, Arg8, Arg9, Arg10,
    Arg11)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10, Arg11) const;
 public:
  ConstMemberCallback_5_6(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5,
      PreArg6 pa6):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5), pa_6(pa6) {}
  ~ConstMemberCallback_5_6() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10, Arg11 arg11) {
    CallbackAutoDeleter<ConstMemberCallback_5_6, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6, arg7, arg8,
        arg9, arg10, arg11);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename Arg11, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8, Arg9, Arg10, Arg11)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) const, PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new ConstMemberCallback_5_6<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, PreArg1, PreArg2,
      PreArg3, PreArg4, PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename Arg11, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8, Arg9, Arg10, Arg11)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) const, PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new ConstMemberCallback_5_6<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, PreArg1, PreArg2,
      PreArg3, PreArg4, PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename Arg8,
      typename Arg9, typename Arg10, typename Arg11, typename PreArg1,
      typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5,
      typename PreArg6>
class FunctionCallback_5_6 : public Callback<R(Arg7, Arg8, Arg9, Arg10,
    Arg11)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8,
      Arg9, Arg10, Arg11);
 public:
  FunctionCallback_5_6(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4),
          pa_5(pa5), pa_6(pa6) {}
  FunctionCallback_5_6() {
    function_ = NULL;
  }
  virtual R Run(Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10, Arg11 arg11) {
    CallbackAutoDeleter<FunctionCallback_5_6, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6, arg7, arg8, arg9,
        arg10, arg11);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename Arg10, typename Arg11, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5,
    typename PreArg6>
Callback<R(Arg7, Arg8, Arg9, Arg10, Arg11)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9,
    Arg10, Arg11), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4,
    PreArg5 pa5, PreArg6 pa6) {
  return new FunctionCallback_5_6<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10, Arg11, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5, PreArg6>(function, pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename Arg10, typename Arg11, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5,
    typename PreArg6>
Callback<R(Arg7, Arg8, Arg9, Arg10, Arg11)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,
    Arg8, Arg9, Arg10, Arg11), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new FunctionCallback_5_6<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10, Arg11, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5, PreArg6>(function, pa1, pa2, pa3, pa4, pa5, pa6);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6 >
class MemberCallback_6_0 : public Callback<R(Arg1, Arg2, Arg3, Arg4, Arg5,
    Arg6)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
 public:
  MemberCallback_6_0(Class *object, MemberSignature member):
      object_(object), member_(member) {}
  ~MemberCallback_6_0() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5,
      Arg6 arg6) {
    CallbackAutoDeleter<MemberCallback_6_0, SelfDelete> self_deleter(this);
    return (object_->*member_)(arg1, arg2, arg3, arg4, arg5, arg6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
Callback<R(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6)) {
  return new MemberCallback_6_0<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6>(
      object, member);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
Callback<R(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6)) {
  return new MemberCallback_6_0<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6>(
      object, member);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6 >
class ConstMemberCallback_6_0 : public Callback<R(Arg1, Arg2, Arg3, Arg4, Arg5,
    Arg6)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5,
      Arg6) const;
 public:
  ConstMemberCallback_6_0(const Class *object, MemberSignature member):
      object_(object), member_(member) {}
  ~ConstMemberCallback_6_0() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5,
      Arg6 arg6) {
    CallbackAutoDeleter<ConstMemberCallback_6_0, SelfDelete> self_deleter(this);
    return (object_->*member_)(arg1, arg2, arg3, arg4, arg5, arg6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
Callback<R(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6) const) {
  return new ConstMemberCallback_6_0<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6>(
      object, member);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
Callback<R(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6) const) {
  return new ConstMemberCallback_6_0<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6>(
      object, member);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6>
class FunctionCallback_6_0 : public Callback<R(Arg1, Arg2, Arg3, Arg4, Arg5,
    Arg6)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);
 public:
  FunctionCallback_6_0(FunctionType function) :
      function_(function) {}
  FunctionCallback_6_0() {
    function_ = NULL;
  }
  virtual R Run(Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5,
      Arg6 arg6) {
    CallbackAutoDeleter<FunctionCallback_6_0, SelfDelete> self_deleter(this);
    return function_(arg1, arg2, arg3, arg4, arg5, arg6);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6>
Callback<R(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)) {
  return new FunctionCallback_6_0<true, R, Arg1, Arg2, Arg3, Arg4, Arg5,
      Arg6>(function);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6>
Callback<R(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)) {
  return new FunctionCallback_6_0<false, R, Arg1, Arg2, Arg3, Arg4, Arg5,
      Arg6>(function);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7 ,
      typename PreArg1>
class MemberCallback_6_1 : public Callback<R(Arg2, Arg3, Arg4, Arg5, Arg6,
    Arg7)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7);
 public:
  MemberCallback_6_1(Class *object, MemberSignature member, PreArg1 pa1):
      object_(object), member_(member), pa_1(pa1) {}
  ~MemberCallback_6_1() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6,
      Arg7 arg7) {
    CallbackAutoDeleter<MemberCallback_6_1, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, arg2, arg3, arg4, arg5, arg6, arg7);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1>
Callback<R(Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7), PreArg1 pa1) {
  return new MemberCallback_6_1<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, PreArg1>(
      object, member, pa1);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1>
Callback<R(Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7), PreArg1 pa1) {
  return new MemberCallback_6_1<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, PreArg1>(
      object, member, pa1);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7 ,
      typename PreArg1>
class ConstMemberCallback_6_1 : public Callback<R(Arg2, Arg3, Arg4, Arg5, Arg6,
    Arg7)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7) const;
 public:
  ConstMemberCallback_6_1(const Class *object, MemberSignature member,
      PreArg1 pa1):
      object_(object), member_(member), pa_1(pa1) {}
  ~ConstMemberCallback_6_1() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6,
      Arg7 arg7) {
    CallbackAutoDeleter<ConstMemberCallback_6_1, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, arg2, arg3, arg4, arg5, arg6, arg7);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1>
Callback<R(Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7) const, PreArg1 pa1) {
  return new ConstMemberCallback_6_1<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, PreArg1>(
      object, member, pa1);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename PreArg1>
Callback<R(Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7) const, PreArg1 pa1) {
  return new ConstMemberCallback_6_1<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, PreArg1>(
      object, member, pa1);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename PreArg1>
class FunctionCallback_6_1 : public Callback<R(Arg2, Arg3, Arg4, Arg5, Arg6,
    Arg7)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);
 public:
  FunctionCallback_6_1(FunctionType function, PreArg1 pa1) :
      function_(function), pa_1(pa1) {}
  FunctionCallback_6_1() {
    function_ = NULL;
  }
  virtual R Run(Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6,
      Arg7 arg7) {
    CallbackAutoDeleter<FunctionCallback_6_1, SelfDelete> self_deleter(this);
    return function_(pa_1, arg2, arg3, arg4, arg5, arg6, arg7);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7,
    typename PreArg1>
Callback<R(Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7),
    PreArg1 pa1) {
  return new FunctionCallback_6_1<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, PreArg1>(function, pa1);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7,
    typename PreArg1>
Callback<R(Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7),
    PreArg1 pa1) {
  return new FunctionCallback_6_1<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, PreArg1>(function, pa1);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8 , typename PreArg1, typename PreArg2>
class MemberCallback_6_2 : public Callback<R(Arg3, Arg4, Arg5, Arg6, Arg7,
    Arg8)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8);
 public:
  MemberCallback_6_2(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2) {}
  ~MemberCallback_6_2() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7,
      Arg8 arg8) {
    CallbackAutoDeleter<MemberCallback_6_2, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, arg3, arg4, arg5, arg6, arg7, arg8);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7, Arg8), PreArg1 pa1, PreArg2 pa2) {
  return new MemberCallback_6_2<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8), PreArg1 pa1, PreArg2 pa2) {
  return new MemberCallback_6_2<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8 , typename PreArg1, typename PreArg2>
class ConstMemberCallback_6_2 : public Callback<R(Arg3, Arg4, Arg5, Arg6, Arg7,
    Arg8)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8) const;
 public:
  ConstMemberCallback_6_2(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2) {}
  ~ConstMemberCallback_6_2() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7,
      Arg8 arg8) {
    CallbackAutoDeleter<ConstMemberCallback_6_2, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, arg3, arg4, arg5, arg6, arg7, arg8);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8) const, PreArg1 pa1, PreArg2 pa2) {
  return new ConstMemberCallback_6_2<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) const, PreArg1 pa1, PreArg2 pa2) {
  return new ConstMemberCallback_6_2<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, PreArg1, PreArg2>(
      object, member, pa1, pa2);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename Arg8,
      typename PreArg1, typename PreArg2>
class FunctionCallback_6_2 : public Callback<R(Arg3, Arg4, Arg5, Arg6, Arg7,
    Arg8)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);
 public:
  FunctionCallback_6_2(FunctionType function, PreArg1 pa1, PreArg2 pa2) :
      function_(function), pa_1(pa1), pa_2(pa2) {}
  FunctionCallback_6_2() {
    function_ = NULL;
  }
  virtual R Run(Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7,
      Arg8 arg8) {
    CallbackAutoDeleter<FunctionCallback_6_2, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, arg3, arg4, arg5, arg6, arg7, arg8);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8),
    PreArg1 pa1, PreArg2 pa2) {
  return new FunctionCallback_6_2<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, PreArg1, PreArg2>(function, pa1, pa2);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename PreArg1, typename PreArg2>
Callback<R(Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,
    Arg8), PreArg1 pa1, PreArg2 pa2) {
  return new FunctionCallback_6_2<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, PreArg1, PreArg2>(function, pa1, pa2);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9 , typename PreArg1, typename PreArg2,
      typename PreArg3>
class MemberCallback_6_3 : public Callback<R(Arg4, Arg5, Arg6, Arg7, Arg8,
    Arg9)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9);
 public:
  MemberCallback_6_3(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  ~MemberCallback_6_3() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8,
      Arg9 arg9) {
    CallbackAutoDeleter<MemberCallback_6_3, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, arg4, arg5, arg6, arg7, arg8,
        arg9);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename PreArg1,
    typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7, Arg8, Arg9), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new MemberCallback_6_3<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename PreArg1,
    typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9), PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3) {
  return new MemberCallback_6_3<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9 , typename PreArg1, typename PreArg2,
      typename PreArg3>
class ConstMemberCallback_6_3 : public Callback<R(Arg4, Arg5, Arg6, Arg7, Arg8,
    Arg9)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9) const;
 public:
  ConstMemberCallback_6_3(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  ~ConstMemberCallback_6_3() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8,
      Arg9 arg9) {
    CallbackAutoDeleter<ConstMemberCallback_6_3, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, arg4, arg5, arg6, arg7, arg8,
        arg9);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename PreArg1,
    typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) const, PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3) {
  return new ConstMemberCallback_6_3<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename PreArg1,
    typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) const, PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3) {
  return new ConstMemberCallback_6_3<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3>(
      object, member, pa1, pa2, pa3);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename Arg8,
      typename Arg9, typename PreArg1, typename PreArg2, typename PreArg3>
class FunctionCallback_6_3 : public Callback<R(Arg4, Arg5, Arg6, Arg7, Arg8,
    Arg9)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8,
      Arg9);
 public:
  FunctionCallback_6_3(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3) {}
  FunctionCallback_6_3() {
    function_ = NULL;
  }
  virtual R Run(Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8,
      Arg9 arg9) {
    CallbackAutoDeleter<FunctionCallback_6_3, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, arg4, arg5, arg6, arg7, arg8, arg9);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8,
    Arg9), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new FunctionCallback_6_3<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3>(function, pa1, pa2, pa3);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename PreArg1, typename PreArg2, typename PreArg3>
Callback<R(Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,
    Arg8, Arg9), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3) {
  return new FunctionCallback_6_3<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, PreArg1, PreArg2, PreArg3>(function, pa1, pa2, pa3);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9, typename Arg10 , typename PreArg1,
      typename PreArg2, typename PreArg3, typename PreArg4>
class MemberCallback_6_4 : public Callback<R(Arg5, Arg6, Arg7, Arg8, Arg9,
    Arg10)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10);
 public:
  MemberCallback_6_4(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4) {}
  ~MemberCallback_6_4() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9,
      Arg10 arg10) {
    CallbackAutoDeleter<MemberCallback_6_4, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, arg5, arg6, arg7, arg8,
        arg9, arg10);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4) {
  return new MemberCallback_6_4<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, PreArg1, PreArg2, PreArg3,
      PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10), PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4) {
  return new MemberCallback_6_4<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, PreArg1, PreArg2, PreArg3,
      PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9, typename Arg10 , typename PreArg1,
      typename PreArg2, typename PreArg3, typename PreArg4>
class ConstMemberCallback_6_4 : public Callback<R(Arg5, Arg6, Arg7, Arg8, Arg9,
    Arg10)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10) const;
 public:
  ConstMemberCallback_6_4(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4) {}
  ~ConstMemberCallback_6_4() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9,
      Arg10 arg10) {
    CallbackAutoDeleter<ConstMemberCallback_6_4, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, arg5, arg6, arg7, arg8,
        arg9, arg10);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) const, PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4) {
  return new ConstMemberCallback_6_4<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, PreArg1, PreArg2,
      PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10) const, PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new ConstMemberCallback_6_4<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, PreArg1, PreArg2,
      PreArg3, PreArg4>(
      object, member, pa1, pa2, pa3, pa4);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename Arg8,
      typename Arg9, typename Arg10, typename PreArg1, typename PreArg2,
      typename PreArg3, typename PreArg4>
class FunctionCallback_6_4 : public Callback<R(Arg5, Arg6, Arg7, Arg8, Arg9,
    Arg10)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8,
      Arg9, Arg10);
 public:
  FunctionCallback_6_4(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4) {}
  FunctionCallback_6_4() {
    function_ = NULL;
  }
  virtual R Run(Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9,
      Arg10 arg10) {
    CallbackAutoDeleter<FunctionCallback_6_4, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, arg5, arg6, arg7, arg8, arg9,
        arg10);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename Arg10, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9,
    Arg10), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new FunctionCallback_6_4<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10, PreArg1, PreArg2, PreArg3, PreArg4>(function,
      pa1, pa2, pa3, pa4);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename Arg10, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4>
Callback<R(Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,
    Arg8, Arg9, Arg10), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4) {
  return new FunctionCallback_6_4<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10, PreArg1, PreArg2, PreArg3, PreArg4>(function,
      pa1, pa2, pa3, pa4);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9, typename Arg10, typename Arg11 ,
      typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
      typename PreArg5>
class MemberCallback_6_5 : public Callback<R(Arg6, Arg7, Arg8, Arg9, Arg10,
    Arg11)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10, Arg11);
 public:
  MemberCallback_6_5(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5) {}
  ~MemberCallback_6_5() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10,
      Arg11 arg11) {
    CallbackAutoDeleter<MemberCallback_6_5, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, arg6, arg7, arg8,
        arg9, arg10, arg11);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename Arg11, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7, Arg8, Arg9, Arg10, Arg11)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11), PreArg1 pa1, PreArg2 pa2,
    PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new MemberCallback_6_5<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, PreArg1, PreArg2,
      PreArg3, PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename Arg11, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7, Arg8, Arg9, Arg10, Arg11)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11), PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new MemberCallback_6_5<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, PreArg1, PreArg2,
      PreArg3, PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9, typename Arg10, typename Arg11 ,
      typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
      typename PreArg5>
class ConstMemberCallback_6_5 : public Callback<R(Arg6, Arg7, Arg8, Arg9,
    Arg10, Arg11)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10, Arg11) const;
 public:
  ConstMemberCallback_6_5(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5) {}
  ~ConstMemberCallback_6_5() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10,
      Arg11 arg11) {
    CallbackAutoDeleter<ConstMemberCallback_6_5, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, arg6, arg7, arg8,
        arg9, arg10, arg11);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename Arg11, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7, Arg8, Arg9, Arg10, Arg11)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) const, PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new ConstMemberCallback_6_5<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, PreArg1, PreArg2,
      PreArg3, PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename Arg11, typename PreArg1, typename PreArg2, typename PreArg3,
    typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7, Arg8, Arg9, Arg10, Arg11)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11) const, PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) {
  return new ConstMemberCallback_6_5<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, PreArg1, PreArg2,
      PreArg3, PreArg4, PreArg5>(
      object, member, pa1, pa2, pa3, pa4, pa5);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename Arg8,
      typename Arg9, typename Arg10, typename Arg11, typename PreArg1,
      typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5>
class FunctionCallback_6_5 : public Callback<R(Arg6, Arg7, Arg8, Arg9, Arg10,
    Arg11)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8,
      Arg9, Arg10, Arg11);
 public:
  FunctionCallback_6_5(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4, PreArg5 pa5) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4),
          pa_5(pa5) {}
  FunctionCallback_6_5() {
    function_ = NULL;
  }
  virtual R Run(Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10,
      Arg11 arg11) {
    CallbackAutoDeleter<FunctionCallback_6_5, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, pa_5, arg6, arg7, arg8, arg9,
        arg10, arg11);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename Arg10, typename Arg11, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7, Arg8, Arg9, Arg10, Arg11)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9,
    Arg10, Arg11), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4,
    PreArg5 pa5) {
  return new FunctionCallback_6_5<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10, Arg11, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5>(function, pa1, pa2, pa3, pa4, pa5);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename Arg10, typename Arg11, typename PreArg1,
    typename PreArg2, typename PreArg3, typename PreArg4, typename PreArg5>
Callback<R(Arg6, Arg7, Arg8, Arg9, Arg10, Arg11)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,
    Arg8, Arg9, Arg10, Arg11), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5) {
  return new FunctionCallback_6_5<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10, Arg11, PreArg1, PreArg2, PreArg3, PreArg4,
      PreArg5>(function, pa1, pa2, pa3, pa4, pa5);
}

/////////////////////////////////////////////////////////////////////////////
// Class member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9, typename Arg10, typename Arg11,
      typename Arg12 , typename PreArg1, typename PreArg2, typename PreArg3,
      typename PreArg4, typename PreArg5, typename PreArg6>
class MemberCallback_6_6 : public Callback<R(Arg7, Arg8, Arg9, Arg10, Arg11,
    Arg12)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10, Arg11, Arg12);
 public:
  MemberCallback_6_6(Class *object, MemberSignature member, PreArg1 pa1,
      PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5), pa_6(pa6) {}
  ~MemberCallback_6_6() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10, Arg11 arg11,
      Arg12 arg12) {
    CallbackAutoDeleter<MemberCallback_6_6, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6, arg7, arg8,
        arg9, arg10, arg11, arg12);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename Arg11, typename Arg12, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8, Arg9, Arg10, Arg11, Arg12)>*
NewCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3, Arg4,
    Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12), PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new MemberCallback_6_6<true, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, PreArg1,
      PreArg2, PreArg3, PreArg4, PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename Arg11, typename Arg12, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8, Arg9, Arg10, Arg11, Arg12)>*
NewPermanentCallback(Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12), PreArg1 pa1,
    PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new MemberCallback_6_6<false, R, Class, CallerClass, Arg1, Arg2, Arg3,
      Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, PreArg1,
      PreArg2, PreArg3, PreArg4, PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

/////////////////////////////////////////////////////////////////////////////
// Class const member callbacks

template <
  bool SelfDelete,
  typename R,
  typename Class,
  typename CallerClass, typename Arg1, typename Arg2, typename Arg3,
      typename Arg4, typename Arg5, typename Arg6, typename Arg7,
      typename Arg8, typename Arg9, typename Arg10, typename Arg11,
      typename Arg12 , typename PreArg1, typename PreArg2, typename PreArg3,
      typename PreArg4, typename PreArg5, typename PreArg6>
class ConstMemberCallback_6_6 : public Callback<R(Arg7, Arg8, Arg9, Arg10,
    Arg11, Arg12)> {
  typedef R (CallerClass::*MemberSignature)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) const;
 public:
  ConstMemberCallback_6_6(const Class *object, MemberSignature member,
      PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5,
      PreArg6 pa6):
      object_(object), member_(member), pa_1(pa1), pa_2(pa2), pa_3(pa3),
          pa_4(pa4), pa_5(pa5), pa_6(pa6) {}
  ~ConstMemberCallback_6_6() {
    object_ = NULL;
    member_ = NULL;
  }
  virtual R Run(Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10, Arg11 arg11,
      Arg12 arg12) {
    CallbackAutoDeleter<ConstMemberCallback_6_6, SelfDelete> self_deleter(this);
    return (object_->*member_)(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6, arg7, arg8,
        arg9, arg10, arg11, arg12);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  const Class* object_;
  MemberSignature member_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename Arg11, typename Arg12, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8, Arg9, Arg10, Arg11, Arg12)>*
NewCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2, Arg3,
    Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) const,
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5,
    PreArg6 pa6) {
  return new ConstMemberCallback_6_6<true, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, PreArg1,
      PreArg2, PreArg3, PreArg4, PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Class, typename CallerClass, typename Arg1,
    typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6,
    typename Arg7, typename Arg8, typename Arg9, typename Arg10,
    typename Arg11, typename Arg12, typename PreArg1, typename PreArg2,
    typename PreArg3, typename PreArg4, typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8, Arg9, Arg10, Arg11, Arg12)>*
NewPermanentCallback(const Class *object, R (CallerClass::*member)(Arg1, Arg2,
    Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12) const,
    PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4, PreArg5 pa5,
    PreArg6 pa6) {
  return new ConstMemberCallback_6_6<false, R, Class, CallerClass, Arg1, Arg2,
      Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, PreArg1,
      PreArg2, PreArg3, PreArg4, PreArg5, PreArg6>(
      object, member, pa1, pa2, pa3, pa4, pa5, pa6);
}

//////////////////////////////////////////////////////////////////////////////
// Normal function callbacks

template <
  bool SelfDelete,
  typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
      typename Arg5, typename Arg6, typename Arg7, typename Arg8,
      typename Arg9, typename Arg10, typename Arg11, typename Arg12,
      typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
      typename PreArg5, typename PreArg6>
class FunctionCallback_6_6 : public Callback<R(Arg7, Arg8, Arg9, Arg10, Arg11,
    Arg12)> {
  typedef R (*FunctionType)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8,
      Arg9, Arg10, Arg11, Arg12);
 public:
  FunctionCallback_6_6(FunctionType function, PreArg1 pa1, PreArg2 pa2,
      PreArg3 pa3, PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) :
      function_(function), pa_1(pa1), pa_2(pa2), pa_3(pa3), pa_4(pa4),
          pa_5(pa5), pa_6(pa6) {}
  FunctionCallback_6_6() {
    function_ = NULL;
  }
  virtual R Run(Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10, Arg11 arg11,
      Arg12 arg12) {
    CallbackAutoDeleter<FunctionCallback_6_6, SelfDelete> self_deleter(this);
    return function_(pa_1, pa_2, pa_3, pa_4, pa_5, pa_6, arg7, arg8, arg9,
        arg10, arg11, arg12);
  }
  virtual bool IsRepeatable() const { return !SelfDelete; }
 private:
  FunctionType function_;
  typename std::tr1::remove_reference<PreArg1>::type pa_1;
  typename std::tr1::remove_reference<PreArg2>::type pa_2;
  typename std::tr1::remove_reference<PreArg3>::type pa_3;
  typename std::tr1::remove_reference<PreArg4>::type pa_4;
  typename std::tr1::remove_reference<PreArg5>::type pa_5;
  typename std::tr1::remove_reference<PreArg6>::type pa_6;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename Arg10, typename Arg11, typename Arg12,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8, Arg9, Arg10, Arg11, Arg12)>*
NewCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9,
    Arg10, Arg11, Arg12), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3, PreArg4 pa4,
    PreArg5 pa5, PreArg6 pa6) {
  return new FunctionCallback_6_6<true, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, PreArg1, PreArg2, PreArg3,
      PreArg4, PreArg5, PreArg6>(function, pa1, pa2, pa3, pa4, pa5, pa6);
}

template <typename R, typename Arg1, typename Arg2, typename Arg3,
    typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8,
    typename Arg9, typename Arg10, typename Arg11, typename Arg12,
    typename PreArg1, typename PreArg2, typename PreArg3, typename PreArg4,
    typename PreArg5, typename PreArg6>
Callback<R(Arg7, Arg8, Arg9, Arg10, Arg11, Arg12)>*
NewPermanentCallback(R (*function)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,
    Arg8, Arg9, Arg10, Arg11, Arg12), PreArg1 pa1, PreArg2 pa2, PreArg3 pa3,
    PreArg4 pa4, PreArg5 pa5, PreArg6 pa6) {
  return new FunctionCallback_6_6<false, R, Arg1, Arg2, Arg3, Arg4, Arg5, Arg6,
      Arg7, Arg8, Arg9, Arg10, Arg11, Arg12, PreArg1, PreArg2, PreArg3,
      PreArg4, PreArg5, PreArg6>(function, pa1, pa2, pa3, pa4, pa5, pa6);
}

#endif  // CALLBACK_H_
