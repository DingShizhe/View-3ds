#!/usr/bin/env bash

src_dir='3ds_files/'
des_dir='outputs/'

for f in `ls 3ds_files`; do
    if [[ ${f#*.} == 3ds ]]; then
        # echo ${src_dir}${f}
        # echo ${des_dir}${f%3ds}png
        ./a.out ${src_dir}${f} -o ${des_dir}${f%.3ds}_ph.png
    fi
done
