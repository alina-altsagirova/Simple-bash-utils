#!/bin/bash

# shellcheck disable=SC1091
source ./bash_test/gen_combinations.sh

CHECK_VALGRIND=${CHECK_VALGRIND:-0}

SUCCESS=0 FAILURE=0 COUNTER=0 VALGRIND_FAIL=0

s21_out="s21_log.txt"
sys_out="orig_log.txt"
s21_err="s21_err.txt"
sys_err="sys_err.txt"


run_test() {
    local name="$1"
    local test_arguments="$2"

    if [[ "$3" == "cat" ]]; then
        local my_util="./s21_cat" 
        local sys_util="cat"
    elif [[ "$3" == "grep" ]]; then
        local my_util="./s21_grep"
        local sys_util="grep"
    else 
        exit 1
    fi

    ((COUNTER++))

    clean_args=$(echo "$test_arguments" | tr -d '\n')

    if [[ "$CHECK_VALGRIND" == 1 ]]; then 
    printf 'Valgrind %s .............................' "$clean_args"
       if ! timeout 3 valgrind --leak-check=full --error-exitcode=1 "$my_util" $clean_args >"$s21_out" 2>/dev/null;  then
            ((VALGRIND_FAIL++))
            printf "${red}Valgrind FAIL\n${nc}"
            rm -f $s21_out $sys_out
            return
       else 
            ((SUCCESS++))
            printf "${green}OK\n${nc}"
       fi
    else 
    printf "%d ${purple}%s${nc} %s \n" "$COUNTER" "$my_util" "$clean_args" 

    if ! timeout 2 "$my_util" $clean_args >"$s21_out" 2>"$s21_err" ; then
        ((FAILURE++))
        printf "${red}FAIL TIMEOUT\n${nc}"
        rm -f $s21_out $sys_out
        return
    fi

    printf "   ${blue}%s${nc} %s \n" "$sys_util" "$clean_args" 
     if ! timeout 2 "$sys_util" $clean_args >"$sys_out" 2>"$sys_err" ; then
        ((FAILURE++))
     printf "${red}FAIL TIMEOUT\n${nc}"
        rm -f $s21_out $sys_out
        return
    fi

    if diff -q "$s21_out" "$sys_out" >/dev/null 2>&1 && diff -e "$s21_err" "$sys_err" >/dev/null 2>&1; then 
        printf "..............................................................................diff ${green}OK${nc}\n"
        ((SUCCESS++))
    else
        printf "..............................................................................diff ${red}FAIL${nc}\n"
        diff -u "$sys_out" "$s21_out" >diff_log.txt
        ((FAILURE++))
    fi

    fi
   rm -f "$s21_out" "$sys_out"
}

main() {
    local util="$1"

    red='\033[1;31m'
    green='\033[1;37m'
    blue='\033[0;34m'
    purple='\033[0;35m'
    nc='\033[0m'
    

    printf '=========TESTING===========\n'
    while IFS= read -r args; do
        args=$(printf '%s' "$args" | tr -d '\n')
        name="test"
        run_test "$name" "$args" "$util"
    done < <(generate_combinations "$@")
    rm -f "$s21_out" "$sys_out"
    if ! [[ -s "$s21_err" && -s "$sys_err" ]]; then rm -f "$sys_err" "$s21_err"; fi

    if [[ "$CHECK_VALGRIND" == 1 ]]; then
        printf "FAIL: %d ${green}SUCCESS: %d${nc} \n" "$VALGRIND_FAIL" "$SUCCESS"
    else 
        printf "FAIL: %d ${green}SUCCESS: %d${nc} ALL: %d \n" "$FAILURE" "$SUCCESS" "$COUNTER"
    fi
}
main "$@"