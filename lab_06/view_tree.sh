#!/bin/bash

dot_name="out.dot"

mkdir -p graphs
png_name="graphs/$(date +"%Y.%m.%d-%H:%M:%S").png"

if [ $dot_name ]; then
    dot -Tpng "$dot_name" -o "$png_name"
fi

if [ $dot_name ]; then
    xdg-open "$png_name"
fi
