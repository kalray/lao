#!/bin/sh
#
# Script used to convert c (ex-xcc) files with old comments to doxygen-style 
# comments. Usage: ./xcc2doxy.sh old-comment-style.c
#
# /*
#  * Function_name --	Description
#  * ...
#  */
#
# is converted to
#
# /**
#  * Description
#  * ...
#  */
#
#  (Note that the space between `--' and `Description' is a Tab.
#
# Replace also:
#   =word=  by  @c word
#
sed -i.bak \
  -e '/\/\*\s*$/N;s/\/\*\s*\n \* \w\+ --\t/\/\*\*\n \* /' \
  -e 's/=\(\w\+\)\=/@c \1/g' \
  $1
