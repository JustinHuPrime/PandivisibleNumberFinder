#!/usr/bin/env bash

BASE=2
THREADS=4

if [ -z "$1" ]
then
  true
else
  THREADS=$1
fi

echo "Started search through all bases with $THREADS threads"

while true
do
  echo
  ./finder $BASE -t $THREADS
  let BASE+=2
done