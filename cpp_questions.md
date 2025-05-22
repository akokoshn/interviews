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
struct E
{
  E() { std::cout << "1"; }
  E(const E&) { std::cout << "2"; }
  ~E() { std::cout << "3"; }
};

E f()
{
  return E();
}

int main()
{
  f();
}
```
**`Result: 13`**

Result is not used, so no call copy constructor.

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

# References convertion
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
