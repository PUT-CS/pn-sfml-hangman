#!/bin/bash
for file in ../assets/wordlists/*.txt
do
    unix2dos $file
done

