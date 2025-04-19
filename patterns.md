## Creation
# Factory method
Method of super class for create object, which may be overided in the child classes.
```
class magicBox {
...
virtual coin* createCoin() = 0;
};

class euroMagicBox: public magicBox {
...
coin* createCoin() override {
  // create euro
}
};
```
# Abstract factory
Abstract class provides methods for create one or more types of objects, the concrete implementation allow create concrete object for each type
```
class furnitureFactory {
public:
virtual chair createChair() = 0;
virtual table createTable() = 0;
};

class victorianFurnitureFactory: public furnitureFactory {
public:
chair* createChair() override {
  // create victorian chair
}
};
```
# Builder
Build complex object step by step. Different kinds of object can be built with using same steps.
```
class builder {
private:
product* product;
public:
// may be overrided in child classes for support different implemenation of build steps dependes on product
virtual buildStep1();
virtual buildStep2();
virtual buildStep3();
product* get();
};

class director {
...
product* partialBuild() {
  builder.buildStep1();
  return builder.get();
}

product* fullBuild() {
  builder.buildStep1();
  builder.buildStep2();
  builder.buildStep3();
  return builder.get();
}
};
```
# Prototype
Abstract class which provides `clone` method for correct copy each concrete implementation.
No need to change common code.
```
class shape {
public:
virtual shape* clone() = 0;
};

class rect: public shape {
private:
int x,y,x1,y1;
public:
shape* clone() {
  // copy
}
};
```
# Singleton
Guaranty creation exactly one instance of object.
```
class singleton {
private:
static obj* val;
public:
singleton() = delete;
singleton(const singleton&) = delete;
static obj* get() {
  if (val == nullptr) {
    val = new obj();
  }
  return val;
}
};
```
## Structural
## Behavior
