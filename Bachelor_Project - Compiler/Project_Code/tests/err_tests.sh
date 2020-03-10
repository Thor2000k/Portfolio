#!/bin/bash

name=$(basename "$PWD")
com=''
test=''

if [ "$name" == "Project_Code" ]
then
    com='./compiler'
    test='tests/collection'
else
    com='./../compiler'
    test='collection'
fi

for i in $test/C_Err*.src
do
    echo == Testing file $i ==
    c="$com < '"$i"'"
    $(eval "$c")
    echo
done


