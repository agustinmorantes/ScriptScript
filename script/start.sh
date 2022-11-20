#! /bin/bash

set -e
ROOT="$(dirname "$0")"

cd "$ROOT/.."

cat "$1" | ./bin/Compiler
