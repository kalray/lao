#!/bin/sh
#
# Temporary wrapper for open64 / LAO / Tirex pipe
#


## Only for lao_kernels
# Generate tirex files
for s in src/*.c ; do echo "Generationg `basename $s .c`.tirex" ; ~/work/tools/devimage/st200tools/bin/st200cc -O1 -CG:LAO_optim=0 -I inc -X $s ; done 

# Generate assembly files
for t in *.tirex ; do echo "Generating `basename $t .tirex`.s" ; ~/work/lao/build-st200/ECL/st200-lao $t -o `basename $t .tirex`.s ; done

# Generate executable file
echo "Generating executable file"
echo "~/work/tools/devimage/st200tools/bin/st200cc -mcore=st231 *.s -o $1.exe"
~/work/tools/devimage/st200tools/bin/st200cc -mcore=st231 *.s -o $1.exe

# Run file
echo "Running executable"
echo "~/work/tools/devimage/st200tools/bin/st200-runner $1.exe"
~/work/tools/devimage/st200tools/bin/st200-runner $1.exe

# Alternative: run file with trace
# ~/work/tools/devimage/st200tools/bin/st200-runner $1.exe -ttrace

exit 0

toolroot=/work1/fbouchez/tools/devimage/st200tools/bin
file=$1
tirex=`basename $file .c`.tirex

echo "Compiling $file into $tirex using open64"
$toolroot/st200cc -O1 -CG:LAO_optim=0 -o $tirex $*

assembly=`basename $file .c`.s

echo "Compiling $tirex into $assembly"
/work1/fbouchez/lao/build-st200/ECL/st200-lao -o $assembly $tirex

object=`basename $file .c`.o

echo "Compiling $assembly into $object"
$toolroot/st200cc -c -o $object $assembly

