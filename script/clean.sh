#! /bin/bash

set -e
ROOT="$(dirname "$0")"

echo "Relocating script to '$ROOT'..."
cd "$ROOT/.."

cd bin
make clean
cd ..
rm -rf bin
