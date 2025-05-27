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

# Static array
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
