# This is a simple unit test framework written in Cppfront.

https://www.youtube.com/watch?v=LD0RPjjShPw


I tried `gprof` and `gprofng`. 

## `gprofng`

`gprofng` is statistical, it does not provide a exact timing measures. It works fine with Threads. It has GUI and HTML output options.

```bash
gprofng collect app ./out.out
gprofng display text -functions test.1.er
```

> Note: For each time you runn `collect app`, a new folder will be created, e.g.: `test.1.er`,  `test.2.er`,  `test.3.er`... Use the right one to see the results.

The option need that the application was compiled with `-g`.
```bash
gprofng display text -source main test.1.er
```

> `gprofng` worked out of the box in my WSL Linux distro (binutils version 2.41).

> In the version isntall when I created this test, no HTML or GUI were possible.

See also:
https://www.youtube.com/watch?v=LD0RPjjShPw
https://sourceware.org/binutils/docs/gprofng.pdf


## `gprof`

`gprof` instruments the file to be profiled and gives a precise measures of call counts. But I had problems with timing measures. Maybe it was because of `-pthread` option. I heard it is not so good with threads, but I also so some comments it works.
See also:
http://www2.phys.canterbury.ac.nz/dept/docs/manuals/unix/DEC_4.0e_Docs/HTML/APS30DTE/DOCU_017.HTM
https://www.youtube.com/watch?v=zbTtVW64R_I&t=248s

## Build

To build run:

```bash
Mixed mode
$ . ./prof_build.sh
```

This will compile and run.
