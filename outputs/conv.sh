#!/usr/bin/bash

for f in `ls .`; do
    if [[ ${f##*.} == "ppm" ]]; then
        convert $f ../${f%.ppm}.png
    fi
done