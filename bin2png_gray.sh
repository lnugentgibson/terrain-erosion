#!/bin/bash

make bin2ppm_gray
for f
do
  ./bin2ppm_gray -i $f -o ${f%bin}ppm
  convert ${f%bin}ppm ${f%bin}png
done