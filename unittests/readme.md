This example shows how to perform static and dynamic tests

 - static tests are done during compilation time and they are runned by the compiler using ```constexpr``` and  ```consteval``` and tested with ```static_assert```.
 - dynamic tests are the conventional tests. In this example we use [google tested](https://github.com/google/googletest).
 
 ## Prepare google test
 
```bash
git clone https://github.com/google/googletest.git -b v1.13.0
cd googletest        # Main directory of the cloned repository.
mkdir build          # Create a directory to hold the build output.
cd build
cmake ..             # Generate native build scripts for GoogleTest.

make
sudo make install    # Install in /usr/local/ by default

```
More details in [googletest/README.md](https://github.com/google/googletest/blob/main/googletest/README.md).

 