#!/bin/bash

function check() {
		local expected="$1"
		shift
		local actual
		actual="$(_pass _call "$@")"
		local exitcode=$?
		test ${exitcode} -eq 0 || _die
		_pass ./check_double "${expected}" "${actual}"
}

EXE_NAME=treeize
source $(dirname "$0")/test_common.sh
