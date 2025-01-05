#!/bin/bash

# Usage
if [ -z "$2" ]; then
    echo "Usage: $0 command command ..." >&2
    exit 1
fi

# Print real bash command
echo -n "$1"
for i in "${@:2}";do
    echo -n " | $i"
done
echo

# Print pipex command
echo -n "./pipex \"$1\""
for i in "${@:2}"; do
    echo -n " \"$i\""
done
echo
