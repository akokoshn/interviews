https://refactoring.guru/design-patterns/catalog
# Creation
## Factory method
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
## Abstract factory
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
## Builder
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
## Prototype
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
## Singleton
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
# Structural
## Adapter
Used for collaborate objects with incompatible interfaces.
```
class Target {
 public:
  virtual ~Target() = default;

  virtual std::string Request() const {
    return "Target: The default target's behavior.";
  }
};

class Adaptee {
 public:
  std::string SpecificRequest() const {
    return ".eetpadA eht fo roivaheb laicepS";
  }
};

/**
 * The Adapter makes the Adaptee's interface compatible with the Target's
 * interface.
 */
class Adapter : public Target {
 private:
  Adaptee *adaptee_;

 public:
  Adapter(Adaptee *adaptee) : adaptee_(adaptee) {}
  std::string Request() const override {
    std::string to_reverse = this->adaptee_->SpecificRequest();
    std::reverse(to_reverse.begin(), to_reverse.end());
    return "Adapter: (TRANSLATED) " + to_reverse;
  }
};
```
## Bridge
Used for split a large class or a set of closely related classes into two separate hierarchies—abstraction and implementation—which can be developed independently of each other.
Archive it by switching from inheritance to the object composition. 
```
class Implementation {
 public:
  virtual ~Implementation() {}
  virtual std::string OperationImplementation() const = 0;
};

// add as many as you want, `ConcreteImplementations` not dependent on each other
class ConcreteImplementationA : public Implementation {
 public:
  std::string OperationImplementation() const override {
    return "ConcreteImplementationA: Here's the result on the platform A.\n";
  }
};
class ConcreteImplementationB : public Implementation {
 public:
  std::string OperationImplementation() const override {
    return "ConcreteImplementationB: Here's the result on the platform B.\n";
  }
};

class Abstraction {
  /**
   * @var Implementation
   */
 protected:
  Implementation* implementation_;

 public:
  Abstraction(Implementation* implementation) : implementation_(implementation) {
  }

  virtual ~Abstraction() {
  }

  virtual std::string Operation() const {
    return "Abstraction: Base operation with:\n" +
           this->implementation_->OperationImplementation();
  }
};

class ExtendedAbstraction : public Abstraction {
 public:
  ExtendedAbstraction(Implementation* implementation) : Abstraction(implementation) {
  }
  std::string Operation() const override {
    return "ExtendedAbstraction: Extended operation with:\n" +
           this->implementation_->OperationImplementation();
  }
};

void ClientCode(const Abstraction& abstraction) {
  // ...
  std::cout << abstraction.Operation();
  // ...
}
```
## Composite
Compose objects into tree structures and then work with these structures as if they were individual objects.
Make sense only if object's hierarhy can be represented as a tree.
```
class Component {
 protected:
  Component *parent_;

 public:
  virtual ~Component() {}
  void SetParent(Component *parent) {
    this->parent_ = parent;
  }
  Component *GetParent() const {
    return this->parent_;
  }

  virtual void Add(Component *component) {}
  virtual void Remove(Component *component) {}
  
  virtual bool IsComposite() const {
    return false;
  }
  
  virtual std::string Operation() const = 0;
};

class Leaf : public Component {
 public:
  std::string Operation() const override {
    return "Leaf";
  }
};

class Composite : public Component {

 protected:
  std::list<Component *> children_;

 public:
  void Add(Component *component) override {
    this->children_.push_back(component);
    component->SetParent(this);
  }
  
  void Remove(Component *component) override {
    children_.remove(component);
    component->SetParent(nullptr);
  }
  bool IsComposite() const override {
    return true;
  }
  
  std::string Operation() const override {
    std::string result;
    for (const Component *c : children_) {
      if (c == children_.back()) {
        result += c->Operation();
      } else {
        result += c->Operation() + "+";
      }
    }
    return "Branch(" + result + ")";
  }
};
```
## Decarator
Extend behavior with using wrapper (add exra actions to the API mthods).
```
class Component {
 public:
  virtual ~Component() {}
  virtual std::string Operation() const = 0;
};

class ConcreteComponent : public Component {
 public:
  std::string Operation() const override {
    return "ConcreteComponent";
  }
};

class Decorator : public Component {
 protected:
  Component* component_;

 public:
  Decorator(Component* component) : component_(component) {
  }
  std::string Operation() const override {
    return this->component_->Operation();
  }
};

class ConcreteDecoratorA : public Decorator {
 public:
  ConcreteDecoratorA(Component* component) : Decorator(component) {
  }
  std::string Operation() const override {
    return "ConcreteDecoratorA(" + Decorator::Operation() + ")";
  }
};

class ConcreteDecoratorB : public Decorator {
 public:
  ConcreteDecoratorB(Component* component) : Decorator(component) {
  }

  std::string Operation() const override {
    return "ConcreteDecoratorB(" + Decorator::Operation() + ")";
  }
};

void ClientCode(Component* component) {
  // ...
  std::cout << "RESULT: " << component->Operation();
  // ...
}
```
## Facade
Simplify interface of complex object(s)
```
class Subsystem1 {
 public:
  std::string Operation1() const {
    return "Subsystem1: Ready!\n";
  }
  // ...
  std::string OperationN() const {
    return "Subsystem1: Go!\n";
  }
};

class Subsystem2 {
 public:
  std::string Operation1() const {
    return "Subsystem2: Get ready!\n";
  }
  // ...
  std::string OperationZ() const {
    return "Subsystem2: Fire!\n";
  }
};

class Facade {
 protected:
  Subsystem1 *subsystem1_;
  Subsystem2 *subsystem2_;
 public:
  Facade(
      Subsystem1 *subsystem1 = nullptr,
      Subsystem2 *subsystem2 = nullptr) {
    this->subsystem1_ = subsystem1 ?: new Subsystem1;
    this->subsystem2_ = subsystem2 ?: new Subsystem2;
  }
  ~Facade() {
    delete subsystem1_;
    delete subsystem2_;
  }
  std::string Operation() {
    std::string result = "Facade initializes subsystems:\n";
    result += this->subsystem1_->Operation1();
    result += this->subsystem2_->Operation1();
    result += "Facade orders subsystems to perform the action:\n";
    result += this->subsystem1_->OperationN();
    result += this->subsystem2_->OperationZ();
    return result;
  }
};

void ClientCode(Facade *facade) {
  // ...
  std::cout << facade->Operation();
  // ...
}
```
## Flyweight
Used for fit more objects into the available amount of RAM by sharing common parts of state between multiple objects instead of keeping all of the data in each object.
```
struct SharedState
{
    std::string brand_;
    std::string model_;
    std::string color_;

    SharedState(const std::string &brand, const std::string &model, const std::string &color)
        : brand_(brand), model_(model), color_(color)
    {
    }

    friend std::ostream &operator<<(std::ostream &os, const SharedState &ss)
    {
        return os << "[ " << ss.brand_ << " , " << ss.model_ << " , " << ss.color_ << " ]";
    }
};

struct UniqueState
{
    std::string owner_;
    std::string plates_;

    UniqueState(const std::string &owner, const std::string &plates)
        : owner_(owner), plates_(plates)
    {
    }

    friend std::ostream &operator<<(std::ostream &os, const UniqueState &us)
    {
        return os << "[ " << us.owner_ << " , " << us.plates_ << " ]";
    }
};

class Flyweight
{
private:
    SharedState *shared_state_;

public:
    Flyweight(const SharedState *shared_state) : shared_state_(new SharedState(*shared_state))
    {
    }
    Flyweight(const Flyweight &other) : shared_state_(new SharedState(*other.shared_state_))
    {
    }
    ~Flyweight()
    {
        delete shared_state_;
    }
    SharedState *shared_state() const
    {
        return shared_state_;
    }
    void Operation(const UniqueState &unique_state) const
    {
        std::cout << "Flyweight: Displaying shared (" << *shared_state_ << ") and unique (" << unique_state << ") state.\n";
    }
};

class FlyweightFactory
{
private:
    std::unordered_map<std::string, Flyweight> flyweights_;
    std::string GetKey(const SharedState &ss) const
    {
        return ss.brand_ + "_" + ss.model_ + "_" + ss.color_;
    }

public:
    FlyweightFactory(std::initializer_list<SharedState> share_states)
    {
        for (const SharedState &ss : share_states)
        {
            this->flyweights_.insert(std::make_pair<std::string, Flyweight>(this->GetKey(ss), Flyweight(&ss)));
        }
    }

    Flyweight GetFlyweight(const SharedState &shared_state)
    {
        std::string key = this->GetKey(shared_state);
        if (this->flyweights_.find(key) == this->flyweights_.end())
        {
            std::cout << "FlyweightFactory: Can't find a flyweight, creating new one.\n";
            this->flyweights_.insert(std::make_pair(key, Flyweight(&shared_state)));
        }
        else
        {
            std::cout << "FlyweightFactory: Reusing existing flyweight.\n";
        }
        return this->flyweights_.at(key);
    }
    void ListFlyweights() const
    {
        size_t count = this->flyweights_.size();
        std::cout << "\nFlyweightFactory: I have " << count << " flyweights:\n";
        for (std::pair<std::string, Flyweight> pair : this->flyweights_)
        {
            std::cout << pair.first << "\n";
        }
    }
};

// ...
void AddCarToPoliceDatabase(
    FlyweightFactory &ff, const std::string &plates, const std::string &owner,
    const std::string &brand, const std::string &model, const std::string &color)
{
    std::cout << "\nClient: Adding a car to database.\n";
    const Flyweight &flyweight = ff.GetFlyweight({brand, model, color});
    // The client code either stores or calculates extrinsic state and passes it
    // to the flyweight's methods.
    flyweight.Operation({owner, plates});
}
```
## Proxy
A proxy controls access to the original object, allowing you to perform something either before or after the request gets through to the original object.
```
class Subject {
 public:
  virtual void Request() const = 0;
};

class RealSubject : public Subject {
 public:
  void Request() const override {
    std::cout << "RealSubject: Handling request.\n";
  }
};

class Proxy : public Subject {
 private:
  RealSubject *real_subject_;

  bool CheckAccess() const {
    // Some real checks should go here.
    std::cout << "Proxy: Checking access prior to firing a real request.\n";
    return true;
  }
  void LogAccess() const {
    std::cout << "Proxy: Logging the time of request.\n";
  }

 public:
  Proxy(RealSubject *real_subject) : real_subject_(new RealSubject(*real_subject)) {
  }

  ~Proxy() {
    delete real_subject_;
  }
  void Request() const override {
    if (this->CheckAccess()) {
      this->real_subject_->Request();
      this->LogAccess();
    }
  }
};

void ClientCode(const Subject &subject) {
  // ...
  subject.Request();
  // ...
}
```
# Behavior
## Chain of responsibility
## Command
## Iterator
## Mediator
## Memento
## Observer
## State
## Strategy
## Template method
## Visitor
