### constructors

* rule of three - if a class requires a destructor, copy constructor or copy assignment operator, it probably requires all three
* presence of user defined (or default/deleted) destructor, copy constructor or copy assignment operator prevents implicit definition of move constructor and the move assignment operator
    * rule of five - any class for which move semantics are desirable, has to declare all five special member functions (destructor, copy, assignment, move, move assignment)
        * if you define or delete any copy, move, or destructor function, define or delete them all
    * failing to provide a move constructor and move assignment is usually not an error, just a missed optimization opportunity
* rule of zero - classes that have custom destructors, copy/move constructors or copy/move assignment operators should deal exclusively with ownership. other classes should not have custom destructors, copy/move constructors or copy/move assignment operators
* when a base class is intended for polymorphic use, its destructor may have to be declared public and virtual. this blocks implicit moves (and deprecates implicit copies), and so the special member functions have to be declared as defaulted

### memory management

* new/malloc are not compatible (cant free/delete on new/malloc pointers)

