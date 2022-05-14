#!/bin/bash
for file in /assets/wordlists/
do
    unix2dos $file
done
for file in /saves/
do
    unix2dos $file
done

