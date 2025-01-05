#!/bin/bash


# show help
if [ "$1" == "-h" ]; then
    echo "Usage: $0 [command] [outfile?]" >&2
    exit 1
fi

# get outfile
if [ -z "$2" ]; then
    OUT="/dev/stdout"
else
    OUT="$2"
fi

# get command
if [ -z "$1" ]; then
    COMMAND="$(cat)"
else
    COMMAND="$1"
fi

echo "$ $1" >> "$OUT"
eval "$1" 2>>"$OUT" >>"$OUT"
echo "$ " >> "$OUT"

# to test: in ari/nashi x out ari/nashi
