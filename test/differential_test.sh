#!/bin/bash

function check() {
	   local expected="$1"
	   local actual="$(_pass _call "$2")"
	   test $? -eq 0 || _die
	   test "${actual}" = "-0" && actual=0 # workaround without gmp
	   _pass test "${expected}" = "${actual}"
}

EXE_NAME=differential
source $(dirname "$0")/test_common.sh
