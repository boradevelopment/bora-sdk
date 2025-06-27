# BORA Software Development Kit [Not Alpha Yet]
## The official implementation of proper BORA-WASM code for the best performance

Currently, this version of this SDK is barely pushing alpha. It is currently a test.


# STL
* Maps

# Current To Do Lists
- Find proficient WASM compilier as a basis, emscripten is most likely not a good fit for BORA SDK.
- Create a compilier

# Dev Notes - [Emscripten Builds will be used]
## Getting started with Emscripten [WIN32 Only]
- Use the prebuild.bat to install emscripten
- You will see a command prompt
## Building with Emscripten
Want to build your code, go to 3rdparty/emsdk and go to emcommandprompt or whatever
```bash
cd ..
emcc -Oz main.cpp -I"../include" -o "[YOUR_WASM_FILE]"
```

Very hacky and bad i know

