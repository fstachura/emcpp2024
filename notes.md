### constructors

* rule of three - if a class requires a destructor, copy constructor or copy assignment operator, it probably requires all three
* presence of user defined (or default/deleted) destructor, copy constructor or copy assignment operator prevents implicit definition of move constructor and the move assignment operator
    * rule of five - any class for which move semantics are desirable, has to declare all five special member functions (destructor, copy, assignment, move, move assignment)
        * if you define or delete any copy, move, or destructor function, define or delete them all
    * failing to provide a move constructor and move assignment is usually not an error, just a missed optimization opportunity
* rule of zero - classes that have custom destructors, copy/move constructors or copy/move assignment operators should deal exclusively with ownership. other classes should not have custom destructors, copy/move constructors or copy/move assignment operators
* when a base class is intended for polymorphic use, its destructor may have to be declared public and virtual. this blocks implicit moves (and deprecates implicit copies), and so the special member functions have to be declared as defaulted
* move assignment - called when rvalue on the left hand side of an assignment expression
* move constructor - called when object is initialized with rvalue of the same type, including parameter passing with std::move, init with std::move and return when returned value has a move constructor
* default no-parameters constructor is generated only if there are no constructors with parameters
* remember to check for equality with this in move and copy constructors

#### initialization

* braced initialization - avoids implicit narrowing conversions of builtin types
* braced init will call normal constructors if any match and if std::initializer_list constructor is not defined. if any constructor contains std::initializer_list, it will be prioritized when using braced init
* guessing whether to use braced or parenthesis initialization in templated functions that pass args to constructors is generally impossible
* empty braces mean default constructor, not std::initializer_list. use `({})` to force empty std::initializer_list
* everything that can be interpreted as a declaration, has to be interpreted as a function declaration. example: `A a()` - function declaration. use braces instead: `A a{}`

### memory management

* new/malloc are not compatible (cant free/delete on new/malloc pointers)

#### lvalues and rvalues

* lvalue - object that occupies some identifiable section of memory
* rvalue - objects that arent lvalues
* language constructs operating on values take rvalues as arguments (example: +). when you pass lvalue to such construct, it undergoes a implicit lvalue to rvalue conversion
* all lvalues that arent arrays, functions or incomplete types can be converted to to rvalues
* rvalues cannot be converted to lvalues (but lvalues can be produced from rvalues)
    * `*` takes rvalue and produces lvalue
    * `&` takes lvalue and produces rvalue
* `&` also allows to define reference types (lvalue references). non const lvalue references cannot be assigned rvalues
    * constant lvalue references can be assigned rvalues - often used to avoid copying when passing to functions
* rvalues can be cv (const/volatile) qualified 
    * lvalue to rvalue conversion - if t is non-class, then type of rvalue is the cv-unqualified version of T. otherwise, type is T

#### nullptr

* using 0 or NULL instead of nullptr in overload context can fail, because both are interpreted as int, unlike nullptr

### templates

* more specialized templates should come last
* primary template (most general) should be declared first, before any specialization
* dependent (on templates) types have to be prepended with typename
* you can prevent template specializations from being instantiated with delete

#### function template type deduction

* universal reference - && in type deduction context (auto or templated parameters). basically either a r-value or l-value. only for template functions. IN CONTEXT OF CLASS MEMBERS IS AN R-VALUE
* T is a pointer or a reference type
    * if expression is a reference or a pointer, ignore the reference part of the type, then pattern match expr against T 
        * ex. both int and int& will be passed by reference, c/v left in the type
        * won't match r-values (constants in parameters won't work)
    * in case of references, arrays will be passed as arrays
        * `const char[13] => T& = const char (&)[13]`
    * in case of references, function pointers will be passed as a function reference
* T is a universal reference (T&&)
    * expr is lvalue - both t and paramtype are deduced to be lvalue references (const/volatile qualifiers are left)
        * basically both value types and reference types will be passed by a reference
    * expr is rvalue - the first case (T is a pointer or a reference type) applies (?)
        * ex. constants in parameters will be passed as &&
* T is neither a pointer nor a reference
    * argument will be passed by value
    * ignore reference, const/volatile part, then pattern match against parameter type
        * ex. bot int, const int and const int& will be passed by value (int, const and & stripped)
        * `const char* const` will be passed as `const char*` (you can modify the pointer but not the contents)
    * arrays will be passed as pointers
        * `const char[13] => T& = const char*`
    * function references will be passed as a pointer
* tldr
    * during deduction, reference arguments in templates are not treated as references, the reference part is ignored
    * during deduction for universal references, l-values are treated in a special way
    * during deduction for value parameters, const and volatile parts of the expression type are ignored
    * during deduction, array or function pointers are turned into pointers unless template parameter is a reference

#### typename in function return type

```c++
template<typename... Ts>
typename std::tuple_element<0, std::tuple<Ts...>>::type average(Ts... t) {
  return (t + ...)/sizeof...(Ts);
}
```

#### crtp

#### auto deduction

* same as template type deduction (`const auto`, `auto&`, `auto&&` is supported) except that initializer lists are deduced to `std::initializer_list<T>`
* types of initializer lists are not deduced by templates
* auto in return types and lambda args uses template deduction rules
* use static_cast and auto instead of initializer to show intent

#### lambdas

* lambdas assigned to std::function might allocate. lambdas assigned to auto likely wont

#### you can deduce array size

```c++
template<typename T, std::size N>
auto f(T (&)[N]) {
    return N;
}
```

#### display deduced types

```c++
template typename<T>
class TD;

TD<decltype(x)> xt;
```


### decltype

* is the declaration type of a name (or expression)
* `decltype(auto)` - use decltype deduction rules (especially useful for delayed return declaration). references will be references, unlike with normal auto which removes them
* `int x;`, `decltype(x)` - int, `decltype((x))` - int&. l-values more complicated than names are references to l-values. if an l-value expression that is not a name has type T, then decltype is `T&`
* `decltype(auto) f() { int x = 0; return (x); }` - return type evaluates to `int&`!

#### decltype deduction rules

```c++
Object o;
const Object& r = o;
auto o1 = o;            // Object
decltype(auto) o1 = o;  // const Object&
```

#### delayed return type declaration

```c++
template<typename A, typename B>
auto test(A& a, B b) -> decltype(a[b]) {
    ...
}
```

### using

* using can be parametrized, typedef cannot

### enums

* enum class - scoped 
* enum - unscoped, enum members are available globally

### user literals

only the following parameter lists are allowed on literal operators:
* const char*
* unsigned long long int
* long double
(...)

https://en.cppreference.com/w/cpp/language/user_literal

### sources

* https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/#id3
* https://ww2.ii.uj.edu.pl/~kapela/emcpp/
* Scott Meyers - Effective Modern C++ 

