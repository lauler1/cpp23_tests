This is a very simple example showing how to embed an external file in the final application using ```xdd``` tool.

The ```xdd``` tool creates an ```unsigned char []``` array that can be embedded.

Example with a file ```hello.txt``` conbtaining only ```Hello World!```.

```bash
xxd -iE hello.txt
```

will create:

```c++
unsigned char hello_txt[] = {
  0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21,
  0x0a
};
unsigned int hello_txt_len = 13;
```

Note that this is **not** a null terminated string.

```c++
std::string(reinterpret_cast<char*>(readme_md), readme_md_len);
```
## Continuous compilation

This projects also uses continous compilation which compiles automatically the project if ```main.cpp``` is changed.

```bash
{ while true; do ./build.sh; sleep 1; done; }
```

It does not uses ```watch``` command in order not to have the screen cleaned all the time.

See more in the ```build.sh``` file.


