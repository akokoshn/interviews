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

