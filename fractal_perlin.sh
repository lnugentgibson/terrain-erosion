#!/bin/bash -x

make perlin_grad
make bin2ppm_sign
make remap_bin
make bin_range
make add_bin

width=$1
shift
height=$1
shift

i=0
f=1
l=""
for scale
do
  ./perlin_grad -w $width -h $height -c 1024 -t 1 \
    -i rand_dir_${width}_${height}_2_0000_`printf %03d $i`.bin \
    -o perlin_grad_${width}_${height}_${scale}_0000_`printf %03d $i`.bin
  ./remap_bin -i perlin_grad_${width}_${height}_${scale}_0000_000.bin \
    -o perlin_grad_${width}_${height}_${scale}_0000_`printf %03d $i`_c.bin \
    `./bin_range -i perlin_grad_${width}_${height}_${scale}_0000_`printf %03d $i`.bin` \
    -t '-1.0' -u 1.0
  ./bin2ppm_sign -i perlin_grad_${width}_${height}_${scale}_0000_`printf %03d $i`_c.bin \
    -o perlin_grad_${width}_${height}_${scale}_0000_`printf %03d $i`_c.ppm
  convert perlin_grad_${width}_${height}_${scale}_0000_`printf %03d $i`_c.ppm \
    perlin_grad_${width}_${height}_${scale}_0000_`printf %03d $i`_c.png
  if [ $i -gt 0 ]
  then
    I=$i
    let "I=I+1"
    F=`printf %0${I}d`
    ./add_bin -a perlin_grad_1024_0016_s.bin -b perlin_grad_1024_0008_c.bin \
      -m 1.0 -n ${F:0:1}.${F:1} -o $@
  fi
  let "i=i+1"
done
