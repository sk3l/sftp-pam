#!/bin/bash

if [[ $1 == "tesla" ]] && [[ $2 == "abc123" ]]; then
    echo -n "accepted"
else
    echo -n "denies"
fi
