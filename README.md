# self-checksum

Program that calculates the checksum of its executable and read-only
data memory. This is some example code that can be used if one wants
to ensure that the program is not modified due to random bit flips
while it's running or to generate a program identifier of the
currently running program for some purposes.

# Building

    cc --std=c99 -o self-checksum self-checksum.c xxhash.c

Or use the provide CMake build rules.
