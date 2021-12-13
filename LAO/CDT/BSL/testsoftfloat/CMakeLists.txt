#cmake_minimum_required (VERSION 2.6)
#project(testsoftfloat)

add_library(testsoftfloat STATIC
            fail.c
            random.c
            slowfloat.c
            testCases.c
            testLoops.c
            testsoftfloat.c
            writeHex.c
)

# We want testsoftfloat to be built after Flip* has been compiled.
add_dependencies(testsoftfloat BSL)

set(DIR ../../../../k1_build_${CMAKE_BUILD_TYPE})

# We force O1 because with O3 it fails on trayas when using Release
# build_type.
set_target_properties(testsoftfloat PROPERTIES COMPILE_FLAGS  "-Wall -O1 -DBITS64 -D__HAS_SUBNORMAL -D__HAS_TININESSMODE -D__HAS_SIGNALINGNANS -D__HAS_EXCEPTIONFLAGS -I. -I../")
