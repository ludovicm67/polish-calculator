#!/bin/bash

if ! test -x ./pc; then
    echo "Error: no executable \"./pc\"" 1>&2
    exit 1
fi

./pc <test/tests.txt > test/results.tmp.txt 2>/dev/null

if diff test/results.txt test/results.tmp.txt; then
    printf "\033[32mSUCCESS\033[0m\n"
    exit 0
else
    printf "\n\033[31mFAIL\033[0m\n"
    exit 1
fi
