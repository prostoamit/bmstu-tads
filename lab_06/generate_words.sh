#!/bin/bash

file_name="$1"
size="$2"

activate_source="words_generator/venv/bin/activate"

program="words_generator/main.py"

source "$activate_source"

python3 "$program" "$file_name" "$size"

deactivate
