#!/bin/bash

name=$(basename "$PWD")

if [ "$name" == "Project_Code" ]
then
    com='./compiler'
else
    com='./../compiler'
fi

# function for testing one array of inputs with 
# another array of expected results.
performtest() {
    local -n inpA=$1
    local -n out=$2

    local size=${#inpA[*]}
    local count=0

    for i in ${!inpA[*]}
    do
        local inp="${inpA[$i]}"

        local c="$com <<< '"$inp"'"
        local result=$(eval "$c" 2>&1) 

        # replace \n with space " "
        result=${result//$'\n'/ }
        # remove indents
        result=${result//  /}

        local expected=${out[$i]}

        if [ "$result" == "$expected" ] 
        then
            count=$((count + 1))
        else
            echo FAILED!
            printf "%-10s %s\n" "input:" "$inp"
            printf "%-10s %s\n" "expected:" "$expected"
            printf "%-10s %s\n" "result:" "$result"
            echo
        fi
    done

    echo $count/$size tests passed!
    echo  
}

# inp_* input for stdin in compiler.
# out_* expected output in stderr from the compiler.

# == Boolean Precedence Test == 
inp_bool=(
    "var x:boolean; x = true && false && true;" 
    "var x:boolean; x = true || false || true;" 
    "var x:boolean; x = true && false || true;" 
    "var x:boolean; x = true || false && true;")

out_bool=(
    "var x:boolean; x = ((true && false) && true);"
    "var x:boolean; x = ((true || false) || true);"  
    "var x:boolean; x = ((true && false) || true);" 
    "var x:boolean; x = (true || (false && true));")

# == Comparison Association Test == 
op_assoc=(">" "<" ">=" "<=" "==" "!=")
inp_assoc=()
out_assoc=()

for j in ${!op_assoc[*]}
do
    for k in ${!op_assoc[*]}
    do
        n=$(($j * 6 + $k))
        op1=${op_assoc[$j]}
        op2=${op_assoc[$k]}
        inp_assoc[$n]="var x:boolean; x = 10 $op1 20 $op2 30;"
        out_assoc[$n]="syntax error in line 1 before $op2." 
    done
done

# Arithmetic Precedence Test
inp_arith=(
    "var x:int; x = 2 + 3;" 
    "var x:int; x = 2 - 3;" 
    "var x:int; x = 2 * 3;" 
    "var x:int; x = 2 / 3;" 
    "var x:int; x = 2 + 3 + 4;" 
    "var x:int; x = 2 - 3 - 4;" 
    "var x:int; x = 2 * 3 * 4;" 
    "var x:int; x = 2 / 3 / 4;"
    "var x:int; x = 2 + 3 * 4 - 5;" 
    "var x:int; x = 2 - 3 * 4 + 5;" 
    "var x:int; x = 2 + 3 / 4 - 5;" 
    "var x:int; x = 2 - 3 / 4 + 5;"
    "var x:int; x = 2 + 3 * 4 * 5;" 
    "var x:int; x = 2 - 3 * 4 / 5;" 
    "var x:int; x = 2 + 3 / 4 * 5;" 
    "var x:int; x = 2 - 3 / 4 / 5;")

# expected output from compiler
out_arith=(
    "var x:int; x = (2 + 3);" 
    "var x:int; x = (2 - 3);" 
    "var x:int; x = (2 * 3);" 
    "var x:int; x = (2 / 3);"
    "var x:int; x = ((2 + 3) + 4);" 
    "var x:int; x = ((2 - 3) - 4);" 
    "var x:int; x = ((2 * 3) * 4);" 
    "var x:int; x = ((2 / 3) / 4);"
    "var x:int; x = ((2 + (3 * 4)) - 5);" 
    "var x:int; x = ((2 - (3 * 4)) + 5);" 
    "var x:int; x = ((2 + (3 / 4)) - 5);" 
    "var x:int; x = ((2 - (3 / 4)) + 5);"
    "var x:int; x = (2 + ((3 * 4) * 5));" 
    "var x:int; x = (2 - ((3 * 4) / 5));" 
    "var x:int; x = (2 + ((3 / 4) * 5));" 
    "var x:int; x = (2 - ((3 / 4) / 5));")

# Identifier Parsing Test

ids=("_" "__" "_2" "_a" "x" "X" "zyx" "z_yx" "ZyX" "ZYX__")
inp_id=()
out_id=()

for i in ${!ids[*]}
do 
    id="${ids[$i]}"
    inp_id[$i]="var $id:int; write $id;"    
    out_id[$i]="var $id:int; write $id;"
done

id_size=${#ids[@]}
inp_id[$id_size]="var 2:int; write 2;"
out_id[$id_size]="syntax error in line 1 before 2."

# Combination Precedence Test
inp_comb=(
    "var x:int, y:bool; y = 10 < x && 20 > x + 1;"
)

out_comb=(
    "var x:int, y:bool; y = ((10 < x) && (20 > (x + 1)));"
)

# Statement Presednce Test
inp_stat=(
    "var x:bool; if x then if !x then write 1; else write 0;"
    "var x:bool; if x then { if !x then { write 1; } } else { write 0; }"
)

out_stat=(
    "var x:bool; (if x then (if (!x) then write 1; else write 0; ) )"
    "var x:bool; (if x then { (if (!x) then { write 1; } ) } else { write 0; } )"
)

echo == Boolean Precedence Test == 
performtest inp_bool out_bool

echo == Comparison Association Test == 
performtest inp_assoc out_assoc

echo == Arithmetic Precedence Test ==
performtest inp_arith out_arith

echo == Identifier Parsing Test ==
performtest inp_id out_id

echo == Combination Precedence Test ==
performtest inp_comb out_comb

echo == Statement Precedence Test ==
performtest inp_stat out_stat
