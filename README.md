# cpproperties
Properties in C++

## Library Info
Header-only library

Hasn't been meticulously tested, but it should work on any C++17 conforming compiler

### Use Info

Do not declare the property object itself const, declare the type const.
Do:
 `Property<int const> my_int;`

Don't: 
`const Property<int> rule_breaker;`

When specifying a read-only property, mark the type as const. Any setter-related functions will fail to
compile if used.

Do not specify explicit getter/setter return types unless you plan to specify custom getter/setters.

An implicit conversion operator as well as the arrow operator are provided for commodity. Note that:
- Sometimes implicit conversion isn't enough. For that reason, a `Property::get()` method is provided. `static_cast`s are also an option.
- The arrow operator is simply commodity syntax for `my_property.get().XXX`. That is, the previous snippet and `my_property->XXX` are equivalent.

