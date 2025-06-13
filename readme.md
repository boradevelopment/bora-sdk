# BORA Software Development Kit [Not Alpha Yet]
## The official implementation of proper BORA-WASM code for the best performance

Currently, this version of this SDK is barely pushing alpha. It is currently a test.

# Current To Do Lists
- Finish implementating atomics and use it for control blocks in shared and unique pointers
- Find proficient WASM compilier as a basis, emscripten is most likely not a good fit for BORA SDK.
- Create a compilier

# Dev Notes - [Emscripten Builds will most likely be discontinued in the future for much more controlled compiling.]
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

