cmake_minimum_required (VERSION 2.6)
project(self-checksum)

add_executable(self-checksum self-checksum.c xxhash.c)

set_property(TARGET self-checksum PROPERTY C_STANDARD 99)
