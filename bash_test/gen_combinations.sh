#!/bin/bash

generate_combinations() {
    if [[ "$1" == "cat" ]]; then
        local file="./bash_test/s21_cat.txt"
        local flags=(b e n s t v)
        local pattern=""
    elif [[ "$1" == "grep" ]]; then
        local file="./bash_test/grep_test.txt"
        local flags=(e i v c l n h s o)
        local pattern="o"
        echo " -f  ./bash_test/regs.txt $file "

    else 
    exit 1
    fi

    for flag in "${flags[@]}"; do
        echo "-$flag $pattern $file"
    done

    for ((i=1; i<${#flags[@]}; i++)); do
        for((j=i+1; j<${#flags[@]}; j++)); do
            echo " -${flags[i]} -${flags[j]} $pattern $file"
        done
    done

    for ((i=1; i<${#flags[@]}; i++)); do
        for((j=i+1; j<${#flags[@]}; j++)); do
            for((k=j+1; k<${#flags[@]}; k++)); do 
                echo "-${flags[i]} -${flags[j]} -${flags[k]} $pattern $file"
            done
        done
    done

    for flag in "${flags[@]}"; do
        echo "-$flag $pattern $file $file"
    done
    
}
