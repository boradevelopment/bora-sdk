# BORA Software Development Kit [Not Alpha Yet]
## The official implementation of proper BORA-WASM code for the best performance

Currently, this version of this SDK is barely pushing alpha. It is currently a test.

Everything in this project must be built from source, including the C++ SDK and the compiler

# STL
* Maps

# Current To Do Lists
- Update Runtime to use versioning
- Create proper Memory System for heap
- Update Runtime to grow heap size and to not be limited.
- Create BORA Debugging via runtime instructions
- Make compilier build every component [3rdparties]


# Dev Notes - Building
use BORAC, remember to add BORA_SDK_PATH to the system environment variables, its required and essential.

Before using BORAC, install dependents through
```bash
borac install
```

It will do all the work, wait for it to finish.

## How to build test apps on BORAC
To build a BORA application
```bash
borac main.cpp --o="main" 
# in future borac
# borac main.cpp -o main

# It will output main.bapp in your directory
``` 
I'm aware the app params are ugly, i'll fix it tomorrow.

## How to build libbora.bcdep 
Since the BORA-SDK is compiled of borac, you'll need to compile it for tests and etc. How?
```bash
# In cpp/libs
borac ../source/bora.cpp --o="bora" --as="cdep" --nobora
# in future borac 
# borac ../source -o bora -as cdep --nobora

# It will output libbora.bcdep in your directory!
```

So far, so good.

