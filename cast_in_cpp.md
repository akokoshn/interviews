# const cast
Conversion `const T` <=> `T`
- Use not constant mennbers inside constant methods
- Pass constant objects as not constant to the functions
Modification of originally const object leads undefined behavior
```
int main(void)
{
    const int val = 10;
    const int *ptr = &val;
    int *ptr1 = const_cast <int *>(ptr);
    *ptr1 = 11;
    cout << val; // Undefined
    return 0;
}
```
`const_cast` not allow write to the constant memory segment
```
static const int val = 10;
int main(void) {
  const int* p = &val;
  int* p1 = const_cast<int*>(p);
  *p1 = 11; // segmentation fault
  return 0;
}
```
# static_cast
Compile time type convertion.
Can't convert pointers to the different types, even they have same size.
```
char c = 'A';
uint8_t *p = static_cast<uint8_t*>(&c); // compile error: invalid convesion
```
Also can be used for upscale casting, but only in case of public inherit.
```
class Base 
{};

class Derived: public Base 
{ 
  // Inherited private/protected 
  // not public
};

class DerivedPrivate: private Base 
{ 
  // Inherited private/protected 
  // not public
};

// Driver code
int main()
{    
  Derived d0;
  Base* b0 = (Bae*)(&d0); // OK
  Base* b1 = static_cast<Base*>(&d0); // OK
  DerivedPrivate d1;
  Base* b2 = (Bae*)(&d1); // OK
  Base* b3 = static_cast<Base*>(&d1); // compiler error: 'Base' is an inaccessible base of 'DerivedPrivate'
  return 0;
}
```
Downscale casting may leads undefined behavior.
# dynamic_cast
Works for both down/up scale casting, but works only for objects with at least one virtual method,
Returns `nullptr` in case of invalid casting.
Invalid cast to reference leads exception `bad_cast`
# reinterpret_cast`
