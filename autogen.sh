#!/bin/bash

cd "$(dirname "$0")"
test -d m4/generated || mkdir m4/generated
autoreconf -i "$@"
