#!/bin/bash

for i in $(seq -w 1 100); do
  echo -n "tf_$i.txt : "
  cat "./tf_data/tf_$i.txt" | awk '{sum+=$3}END{print sum}'
done