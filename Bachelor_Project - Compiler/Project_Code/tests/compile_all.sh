#!/bin/bash
# requires being run in bash

echo Start compiling...

name=$(basename "$PWD")
com=''
test=''
asm=''
log_file='compile_all.log'

if [ "$name" == "Project_Code" ]
then
    com='./compiler'
    test='tests/collection'
    asm='tests/assembler'
    log_file='tests/logs/compile_all.log'
    mkdir -p 'tests/logs'
else
    com='./../compiler'
    test='collection'
    asm='assembler'
    log_file='logs/compile_all.log'
    mkdir -p 'logs'
fi

rm -rf "$asm"
mkdir -p "$asm"

a="$asm/out.s"     # tmp assembly file
o="$asm/out.o"      # tmp object file
t="$asm/tmp.out"    # tmp ouput file

# create files
echo > $o
echo > $t

echo == Log of All Compiles == > "$log_file"
echo >> "$log_file"

compile_err=0
assembler_err=0
status_err=0
seg_fault=0
wrong_output=0
successful=0

for i in $test/*.src
do
    echo == Compiling file $i ==
    s=$(basename $i)
    s="${s%.*}"         # name of file

    p="$asm/$s"         # executable file
    e="$test/$s.out"    # expected ouput file
    
    c="$com -ph < '"$i"' > '"$a"'"

    echo -ne "\r\e[0KTesting $s" 1>&2

    if output=$(eval "$c" 2>&1) ;
    then    
        echo Compile successful
        if output=$(gcc "$a" -no-pie -m64 -g -o "$p" 2>&1);
        then
            $(eval "./$p > $t")
            status=$?
            if [ $status -eq 0 ]
            then
                d=$(diff "$t" "$e")
                if [ "$d" == "" ]
                then
                    echo Execution successful
                    ((successful++))
                else
                    f="$asm/ERROR_RUN_$s.as"
                    cp "$a" "$f"
                    echo Output unexepected
                    echo $d
                    ((wrong_output++))
                fi
            else
                if [ $status -eq 139 ]
                then
                    ((seg_fault++))
                else
                    ((status_err++))
                fi
                
                echo Execution exited with status $status
                echo Expected
                cat "$e"
            fi
        else
            f="$asm/ERROR_AS_$s.as"
            cp "$a" "$f"
            echo  >> "$f"
            echo Assembler errors >> "$f"
            echo "$output" >> "$f"
            ((assembler_err++))
        fi
    else
        echo Error during compile
        echo "$output"
        echo Expected
        cat "$e"
        ((compile_err++))
    fi
    
    echo
done >> "$log_file"

tput cuu1
echo -e "\e[0K\r"All files compiled, see $log_file for details

total=$((compile_err+assembler_err+status_err+seg_fault+wrong_output+successful))

printf "Total files: %d \nSuccessful: %d" $total $successful 
printf "\nCathed Errors During... \nCompile: %d \nRuntime: %d" $compile_err $status_err
printf "\nOther Errors... \nAssembler: %d \nWrong Output: %d \nSeg Faults: %d\n" $assembler_err $wrong_output $seg_fault