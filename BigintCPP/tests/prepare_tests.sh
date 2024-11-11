#!/bin/bash

cd -- "$(dirname "$0")"

rm -rf ./codegen_output/
mkdir codegen_output
python3.10 generate.py
