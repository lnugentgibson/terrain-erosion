#!/bin/bash

make bin2ppm_col
for f
do
  ./bin2ppm_col -i $f -o ${f%bin}ppm
  convert ${f%bin}ppm ${f%bin}png
done