cpputils [![Build Status](https://travis-ci.org/baabelfish/cpputils.svg?branch=master)](https://travis-ci.org/baabelfish/cpputils)
========

- A little abstraction library for STL and some other stuff.

- Has following functions that work with STL-containers:

```
all
any
areEqual
at
concat
contains
each
filter
find
findLast
first
fold
frequencies
groupBy
keys
last
map
mapValues
max
merge
min
minmax
none
omit
prepend
range
reject
reverse
rfold
size
sort
unique
values
zipMap
zipMultiMap
zipUnorderedMap
zipUnorderedMultiMap
```

- Additional functions:

```
after
defer
delay
identity
listen
once
pipe
tap
times
uniqueId
wait
wrap
```

- TMP-helpers:

```
amount
head
nth
```

- Other:
```
function_traits
```


# Usage
- This is a header-only library so including `cu.hpp` is enough:
```
#include <cpputils/cu.hpp>
#include "path/to/cu.hpp"
```

- You can use the functions directly under `cu` and `cf` -namespaces or
 you can construct chains with `_`-function that constructs `Chain`-objects:

```cpp
auto chain = _({1,2,3,4}).reject([](int i) { return i < 2; })
                         .reverse()
                         .concat(std::list<int>{1,2})
                         .map([](int i) { return i * 2; })
                         .concat(std::vector<int>{3});
Assert().isEqual(chain.values(), { 8, 6, 4, 2, 4, 3 });
Assert().isTrue(chain.contains(8, 6, 3));
```

# Documentation
TODO

# License
MIT
