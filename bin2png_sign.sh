#!/bin/bash

make bin2ppm_sign
for f
do
  ./bin2ppm_sign -i $f -o ${f%bin}ppm
  convert ${f%bin}ppm ${f%bin}png
done