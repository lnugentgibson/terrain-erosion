#!/bin/bash

make bin2ppm_vec
for f
do
  ./bin2ppm_vec -i $f -o ${f%bin}ppm
  convert ${f%bin}ppm ${f%bin}png
done