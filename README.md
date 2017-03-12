# self-checksum

Program that calculates the checksum of its executable and read-only
data memory. This is some example code that can be used if one wants
to ensure that the program is not modified due to random bit flips
while it's running or to generate a program identifier of the
currently running program for some purposes.

This uses 64 bit
[xxHash](https://github.com/Cyan4973/xxHash) library to calculate the
checksum and [glibc](https://www.gnu.org/software/libc/) specific
[dl\_iterate\_phdr](http://man7.org/linux/man-pages/man3/dl_iterate_phdr.3.html)
function to get the loaded shared objects. If you have 32 bit or less
addressable architecture that has no `unsigned long long` type
available, you need to find alternative hash function to 64 bit
version of xxHash. For example 32 bit version of xxHash that also
comes with this. If you have other C standard library than glibc in
use, the working of this program depends on the availability of
`dl_iterate_phdr` function.

## Example

First let's check that what happens when we run this program two times:

    $ ./self-checksum
    d42b66204748ba3c
    $ ./self-checksum
    d42b66204748ba3c

The calculated checksum stays the same even when there is
[address space layout randomization](https://en.wikipedia.org/wiki/Address_space_layout_randomization)
in use, as the actual binary code from dynamically loaded libraries
does not change. Then if we add another library to the program:

    $ echo "void fun() {}" > fun.c
    $ gcc -shared -fPIC fun.c -o fun.so
    $ LD_PRELOAD=$PWD/fun.so ./self-checksum
    9b1e4cafac0ecd41

The calculated checksum changes, as there is now one extra library
with its own data loaded. If we then change the library contents, the
calculated checksum will change:

    $ echo int fun() {return 1;}" > fun.c
    $ gcc -shared -fPIC fun.c -o fun.so
    $ LD_PRELOAD=$PWD/fun.so ./self-checksum
    3234bd960dac2633

## Building

    cc --std=c99 -o self-checksum self-checksum.c xxhash.c

Or use the provide [CMake](https://cmake.org/) build rules.
