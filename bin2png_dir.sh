#!/bin/bash

make bin2ppm_dir
for f
do
  ./bin2ppm_dir -i $f -o ${f%bin}ppm
  convert ${f%bin}ppm ${f%bin}png
done