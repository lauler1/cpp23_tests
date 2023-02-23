This example shows how to use c++ modules with clang++.

It is based on [StandardCPlusPlusModules](https://clang.llvm.org/docs/StandardCPlusPlusModules.html).
 
 ## How to compile
 
```bash
clang++ -std=c++20 -x c++-module hello_test.cpp --precompile -o hello_test.pcm
clang++ -std=c++20 main.cpp -fprebuilt-module-path=. hello_test.pcm -o main.out


```

 