# STL
## MAP
## 1
```
int main()
{
  std::map<bool,int> mb = {{1,2},{3,4},{5,0}};
  std::cout << mb.size();
  std::map<int,int> mi = {{1,2},{3,4},{5,0}};
  std::cout << mi.size();
}
```
**`Result: 13`**

For `mb` all 3 initialize elements have same key = 1, because implicit convertion to `bool`.

## VECTOR
## 1
```
int main() {
  std::vector<char> delimiters = { ",", ";" };
  std::cout << delimiters[0];
}
```
**`Result: runtime exception`**

Try to apply constructor `template <class InputIterator> vector(InputIterator first, InputIterator last)`, then invalid memory access.

# Lambda
## 1
```
struct A {
  A() = default;
  A(const A &a) { std::cout << '1'; }
  A(A &&a) { std::cout << '2'; }
};

A a;

int main() {
  [a = std::move(a)]() {
    [a = std::move(a)]() {};
  }();
}
```
**`Result: 21`**

The outer lambda captured `a` from global scope, it's not const, so used move constructor. The innerlambda captured `a` from the local scope as an constant rvalue reference, `A(std::move(const A& a))` - copy constructor.

## 2
```
void f(int a = []() { static int b = 1; return b++; }())
{
    std::cout << a;
}

int main()
{
    f();
    f();
}
```
**`Result: 12`**

In both calls `f()` used the same instance of lambda.

# Const modifier
## 1
```
struct C {
    C() = default;
    int i;
};

int main() {
    const C c;
    std::cout << c.i;
}
```
**`Result: compile error`**

Because default constructor not allowed for initialize constant.

It may be fixed by definition `C() {}`.

## 2
```
template <typename T>
void foo(T& x)
{
    std::cout << std::is_same_v<const int, T>;
}

template <typename T>
void bar(const T& x)
{
    std::cout << std::is_same_v<const int, T>;
}

int main()
{
    const int i{};
    int j{};

    foo(i);
    foo(j);
    bar(i);
    bar(j);
}
```
**`Result: 1000`**

## 3

```
int main() {
    std::cout << std::is_const_v<const int *>
              << std::is_const_v<const int [1]>
              << std::is_const_v<const int **>
              << std::is_const_v<const int (*)[1]>
              << std::is_const_v<const int *[1]>
              << std::is_const_v<const int [1][1]>;
}
```
**`Result: 010001`**

## 4
```
class C {
public:
  void foo()       { std::cout << "A"; }
  void foo() const { std::cout << "B"; }
};

struct S {
  std::vector<C> v;
  std::unique_ptr<C> u;
  C *const p;

  S()
    : v(1)
    , u(new C())
    , p(u.get())
  {}
};

int main() {
  S s;
  const S &r = s;

  s.v[0].foo();
  s.u->foo();
  s.p->foo();

  r.v[0].foo();
  r.u->foo();
  r.p->foo();
}
```
**`Result: AAABAA`**

However, `r` refers to its object as a const instance of `S`. That constness changes the behavior of its member `v`, an `std::vector` which is "const-correct" in the sense that its `operator[]` returns `const C&` and therefore invokes the const version of `C::foo()`

# Virtual methods
## 1
```
class A {
public:
  virtual void f() { std::cout << "A"; }
};

class B : public A {
public:
  void f() { std::cout << "B"; }
};

void g(A a) { a.f(); }

int main() {
  B b;
  g(b);
}
```
**`Result: A`**

`g()` accepts new temporary object of A (missed original pointer to the v-table)

## 2
```
struct A {
    virtual void foo (int a = 1) {
        std::cout << "A" << a;
    }
};

struct B : A {
    virtual void foo (int a = 2) {
        std::cout << "B" << a;
    }
};

int main () {
    A *b = new B;
    b->foo();
}
```
**`Result: B1`**

# Template
## 1
```
template <typename T>
struct A {
    static_assert(T::value);
};

struct B {
    static constexpr bool value = false;
};

int main() {
    A<B>* a;
    std::cout << 1;
}
```
**`Result: 1`**

No created instance of A.

## 2
```
template<typename T>
void f(T) {
    std::cout << 1;
}

template<>
void f(int) {
    std::cout << 2;
}

void f(int) {
    std::cout << 3;
}

int main() {
    f(0.0);
    f(0);
    f<>(0);
}
```
**`Result: 132`**

## 3
```
struct A {};
struct B {};

template<typename T = A>
struct X;

template<>
struct X<A> {
    static void f() { std::cout << 1; }
};

template<>
struct X<B> {
    static void f() { std::cout << 2; }
};

template<template<typename T = B> class C>
void g() {
    C<>::f();
}

int main() {
    g<X>();
}
```
**`Result: 2`**

A template-parameter of a template template-parameter is permitted to have a default template-argument. When such default arguments are specified, they apply to the template template-parameter in the scope of the template template-parameter.

## 4
```
void f()
{
    std::cout << "1";
}

template<typename T>
struct B
{
    void f()
    {
        std::cout << "2";
    }
};

template<typename T>
struct D : B<T>
{
    void g()
    {
        f();
    }
};

int main()
{
    D<int> d;
    d.g();
}
```
**`Result: 1`**

`f` in the function call `f()` is unqualified and non-dependent, so unqualified name lookup takes place when the template definition is examined, skipping the dependent base class `B` and binding the name to the global declaration of `f`.

## 5
```
template<class T>
void f(T) { std::cout << 1; }

template<>
void f<>(int*) { std::cout << 2; }

template<class T>
void f(T*) { std::cout << 3; }

int main() {
    int *p = nullptr;
    f( p );
}
```
**`Result: 3`**

Fisrt defined template, then specification

## 6
```
template<typename T>
int f()
{
    if constexpr (std::is_same_v<T, int>) { return 0; }
    else { return std::string{}; }
}

int main()
{
    std::cout << f<int>();
}
```
**`Result: undefined behavior`**

"Hypothetical instantiation of a template immediately following its definition would be ill-formed due to a construct that does not depend on a template parameter"

# Scope
## 1
```
namespace x {
  class C {};
  void f(const C& i) {
    std::cout << "1";
  }
}

namespace y {
  void f(const x::C& i) {
    std::cout << "2";
  }
}

int main() {
  f(x::C());
}
```
**`Result: 1`**

## 2
```
namespace A{
  extern "C" int x;
};

namespace B{
  extern "C" int x;
};

int A::x = 0;

int main(){
  std::cout << B::x;
  A::x=1;
  std::cout << B::x;
}
```
**`Result: 01`**

Due to the extern "C" specifications, `A::x` and `B::x` actually refer to the same variable

# Move semantic
## 1
```
int y(int &) { return 1; }
int y(int &&) { return 2; }

template <class T> int f(T &&x) { return y(x); }
template <class T> int g(T &&x) { return y(std::move(x)); }
template <class T> int h(T &&x) { return y(std::forward<T>(x)); }

int main() {
  int i = 10;
  std::cout << f(i) << f(20);
  std::cout << g(i) << g(20);
  std::cout << h(i) << h(20);
  return 0;
}
```
**`Result: 112212`**

# Mutable
## 1
```
struct X {
    X() { std::cout << "1"; }
    X(X &) { std::cout << "2"; }
    X(const X &) { std::cout << "3"; }
    X(X &&) { std::cout << "4"; }
    ~X() { std::cout << "5"; }
};

struct Y {
    mutable X x;
    Y() = default;
    Y(const Y &) = default;
};

int main() {
    Y y1;
    Y y2 = std::move(y1);
}
```
**`Result: 1255`**

Y::x is mutable.

# References
## 1
```
int main() {
    int a = '0';
    char const &b = a;
    //char &b = a; cannot bind non-const lvalue reference of type ‘char&’ to a value of type ‘int’
    //char &&b = a; OK with same result
    std::cout << b;
    a++;
    std::cout << b;
}
```
**`Result: 00`**

`b` initialized by temporary const char.

## 2
```
int main() {
    int i = 1;
    int const& a = i > 0 ? i : 1;
    i = 2;
    std::cout << i << a;
}
```
**`Result: 21`**

Expression `i>0? i : 1` returns temporary value

## 3
```
int main() {
    int x = 10;
    int y = 10;

    const int &max = std::max(x, y);
    const int &min = std::min(x, y);

    x = 11;
    y = 9;

    std::cout << max << min;
}
```
**`Result: 1111`**

`max`, `min` returns reference, not temporary

## 4
```
int main()
{
  int i, &j = i;
  [=]
  {
    std::cout << std::is_same<decltype    ((j)),     int         >::value
              << std::is_same<decltype   (((j))),    int      &  >::value
              << std::is_same<decltype  ((((j)))),   int const&  >::value
              << std::is_same<decltype (((((j))))),  int      && >::value
              << std::is_same<decltype((((((j)))))), int const&& >::value;
  }();
}
```
**`Result: 00100`**

decltype(T) and decltype((T)) has different meanings. The first (which we don't see in this question) is intended to find the type of a particular identifier. The other, which we see in this question, is intended to find the type of an arbitrary expression

# Be careful with type naming
## 1
```
typedef long long ll;

void foo(unsigned ll) {
    std::cout << "1";
}

void foo(unsigned long long) {
    std::cout << "2";
}

int main() {
    foo(2ull);
}
```
**`Result: 2`**

In `foo(unsigned ll)`  `ll` is paraneter of `unsigned` type

# Variant
## 1
```
struct C
{
    C() : i(1){}
    int i;
};

struct D
{
    D() : i(2){}
    int i;
};

int main()
{
    const std::variant<C,D> v;
    std::visit([](const auto& val){ std::cout << val.i; }, v);
}
```

**`Result: 1`**

`std::variants` default constructor constructs a variant holding the value-initialized value of the first alternative.

# Bit-fields structures
## 1
```
struct X {
    int var1 : 3;
    int var2;
};

int main() {
    X x;
    std::cout << (&x.var1 < &x.var2);
}
```
**`Result: compile error`**

The address-of operator & shall not be applied to a bit-field, so there are no pointers to bit-fields.

# Operator=
## 1
```
struct X {
  X() { std::cout << "a"; }
  X(const X &x) { std::cout << "b"; }
  const X &operator=(const X &x) {
    std::cout << "c";
    return *this;
  }
};

int main() {
  X x;
  X y(x);
  X z = y;
  z = x;
}
```
**`Result: abbc`**

# String
## 1
```
void f(const std::string &) { std::cout << 1; }

void f(const void *) { std::cout << 2; }

int main() {
  f("foo");
  const char *bar = "bar";
  f(bar);
}
```
**`Result: 22`**

# Temporary objects lifetime
## 1
```
class C {
public:
  C(int i) : i(i) { std::cout << i; }
  ~C() { std::cout << i + 5; }

private:
  int i;
};

int main() {
  const C &c = C(1);
  C(2);
  C(3);
}
```
**`Result: 127386`**

Temporary returned from `C(2)` and `C(3)` immedatly destoryed, but `const C &c = C(1)` prolongate lifetime of `C(1)` till end of function

## 2
```
char a[2] = "0";

struct a_string {
    a_string() { *a='1'; }
    ~a_string() { *a='0'; }
    const char* c_str() const { return a; }
};

void print(const char* s) { std::cout << s; }
a_string make_string() { return a_string{}; }

int main() {
    a_string s1 = make_string();
    print(s1.c_str());

    const char* s2 = make_string().c_str();
    print(s2);

    print(make_string().c_str());
}

```
**`Result 101`**

# Pointers
## 1
```
size_t get_size_1(int* arr)
{
  return sizeof arr;
}

size_t get_size_2(int arr[])
{
  return sizeof arr;
}

size_t get_size_3(int (&arr)[10])
{
  return sizeof arr;
}

int main()
{
  int array[10];
  // Assume sizeof(int*) != sizeof(int[10])
  std::cout << (sizeof(array) == get_size_1(array));
  std::cout << (sizeof(array) == get_size_2(array));
  std::cout << (sizeof(array) == get_size_3(array));
}
```
**`Result: 001`**

## 2
```
void takes_pointer(int* pointer) {
  if (typeid(pointer) == typeid(int[])) std::cout << 'a';
  if (typeid(pointer) == typeid(int*)) std::cout << 'p';
}

void takes_array(int array[]) {
  if (typeid(array) == typeid(int[])) std::cout << 'a';
  if (typeid(array) == typeid(int*)) std::cout << 'p';
}

int main() {
  int* pointer = nullptr;
  int array[1];

  takes_pointer(array);
  takes_array(pointer);

  std::cout << (typeid(int*) == typeid(int[]));
}
```
**`Result pp0`**

## 3
```
int main() {
  std::cout << 1["ABC"];
}
```
**`Result: B`**

The expression E1[E2] is identical (by definition) to *((E1)+(E2)), in this case *(1 + "ABC")

# Auto
## 1
```
struct A {
    A() { std::cout << "a"; }

    void foo() { std::cout << "1"; }
};

struct B {
    B() { std::cout << "b"; }
    B(const A&) { std::cout << "B"; }

    void foo() { std::cout << "2"; }
};

int main()
{
    auto L = [](auto flag) -> auto {return flag ? A{} : B{};};
    L(true).foo();
    L(false).foo();
}
```
**`Result: aB2b2`**

Type of `flag ? A{} : B{}` is `B`

# Not obvious errors
# 1
```
struct X {
  X() { std::cout << "X"; }
};

struct Y {
  Y(const X &x) { std::cout << "Y"; }
  void f() { std::cout << "f"; }
};

int main() {
  Y y(X());
  y.f();
}
```
**`Result: compile error`**

`Y y(X())` intrpretted as function declaration. For avoid issue `Y y(X{})`

# Ecxeption
## 1
```
struct GeneralException {
  virtual void print() { std::cout << "G"; }
};

struct SpecialException : public GeneralException {
  void print() override { std::cout << "S"; }
};

void f() { throw SpecialException(); }

int main() {
  try {
    f();
  }
  catch (GeneralException e) {
    e.print();
  }
}
```
**`Result G`**

Catch `GeneralException` by value.

## 2
```
struct S {
    S() {
        throw std::runtime_error("");
    }
    ~S() {
        throw std::runtime_error("");
    }
};

int main() {
    std::cout
        << (std::is_nothrow_constructible_v<S> ? 'y' : 'n')
        << (std::is_nothrow_destructible_v<S> ? 'y' : 'n');
}
```
**`Result: ny`**

All functions without `noexcept` may throw exceptions, but destructors and deallocation functions any way `std::is_nothrow_destructible_v`

## 3
```
struct A {
    A(char c) : c_(c) {}
    ~A() { std::cout << c_; }
    char c_;
};

struct Y { ~Y() noexcept(false) { throw std::runtime_error(""); } };

A f() {
    try {
        A a('a');
        Y y;
        A b('b');
        return {'c'};
    } catch (...) {
    }
    return {'d'};
}

int main()
{
    f();
}
```
**`Result: bcad`**

The returned object of type `A` is constructed. Then, the local variable `b` is destroyed. Next, the local variable `y` is destroyed, causing stack unwinding, resulting in the destruction of the returned object, followed by the destruction of the local variable `a`. Finally, the returned object is constructed again at `return {'b'}`
It is worth noting that at the time of writing, neither GCC, Clang nor MSVC conform to the behaviour described in the standard, and instead print `bacd`, `bad` and `bad`s, respectively.

# Copy constructor
## 1
```
class A
{
public:
    A() { std::cout << "A"; }
    A(const A &) { std::cout << "a"; }
};

class B: public virtual A
{
public:
    B() { std::cout << "B"; }
    B(const B &) { std::cout << "b"; }
};

class C: public virtual A
{
public:
    C() { std::cout << "C"; }
    C(const C &) { std::cout << "c"; }
};

class D: B, C
{
public:
    D() { std::cout << "D"; }
    D(const D &) { std::cout << "d"; }
};

int main()
{
    D d1;
    D d2(d1);
}
```
**`Result: ABCDABCd`**

Copy constractor called only for `D`, in case of user-defined copy constructor

# Jump statement
## 1
```
class A {
public:
  A() { std::cout << "a"; }
  ~A() { std::cout << "A"; }
};

int i = 1;

int main() {
label:
  A a;
  if (i--)
    goto label;
}
```
**`Result: aAaA`**

# Declaration variables
## 1
```
struct X {
    X() { std::cout << "1"; }
    X(const X &) { std::cout << "3"; }
    ~X() { std::cout << "2"; }

    void f() { std::cout << "4"; }

} object;

int main() {
    X(object);
    object.f();
}
```
**`Result: 11422`**

The line `X(object)` interpreted as declaration of the new local variable `object` of type `X`

# Operator ordering
## 1
```
int main() {
  int i = 1, j = 1, k = 1;
  std::cout << ++i || ++j && ++k;
  std::cout << i << j << k;
}
```
**`Result: 2211`**

`(cout << ++i) || (++j && ++k)`, `(++j && ++k)` was not evaluated due to `cout << ++i` not 0
