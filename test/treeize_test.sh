#!/bin/bash

function check() {
       local expected="$1"
       local actual="$(_pass _call "$2")"
       test $? -eq 0 || _die
       _pass ./check_double "${expected}" "${actual}"
}

EXE_NAME=treeize
source $(dirname "$0")/test_common.sh
