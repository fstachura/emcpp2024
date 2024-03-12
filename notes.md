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

### memory management

* new/malloc are not compatible (cant free/delete on new/malloc pointers)

### lvalues and rvalues

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

### sources

* https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c/#id3

