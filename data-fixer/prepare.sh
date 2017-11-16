#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 DATASET" >&2
  exit 1
fi

INPUT=$1
./compress < $INPUT | ./l2cc | ./compress > ${INPUT}.l2cc
