#!/bin/bash

function _die() {
	local mess max i line src

	mess=${1:-assertion failed}
	let max=${#BASH_LINENO[@]}-1

	line=${BASH_LINENO[0]}
	src=${BASH_SOURCE[1]}
	echo "$mess at $src line $line" >&2

	i=1
	while [ $i -lt ${max} ]; do
		line=${BASH_LINENO[$i]}
		src=${BASH_SOURCE[`expr $i + 1`]}
		echo "	${FUNCNAME[$i]}() called at ${src} line ${line}" >&2
		let i=$i+1
	done
	exit 1
}

function _call() {
	../src/${EXE_NAME}${EXEEXT} "$@"
}

function check() {
	local expected="$1"
	local actual="$(_pass _call "$2")"
	test $? -eq 0 || _die
	_pass ./check_double "${expected}" "${actual}"
}

function xfail() {
	printf "\e[35m# xfail: %s\e[0m\n" "$*">&2
	"$@" && _die
}

function _pass() {
	printf "\e[36m# _pass:  %s\e[0m\n" "$*" >&2
	"$@" || _die
}

test "${EXE_NAME:-unset}" = 'unset' && _die "EXE_NAME is not set"
test_name=${0%%.sh}
for i in "${srcdir:-.}/${test_name##*/}s"/*_*.sh; do
	printf "\e[1;32mtesting %s...\e[0m\n" "$(basename "$i")"
	source "$i"
done

exit 0
