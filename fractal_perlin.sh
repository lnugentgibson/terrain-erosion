#!/bin/bash -x

make perlin_grad
make bin2ppm_sign
#make remap_bin
#make bin_range
make add_bin

width=$1
shift
height=$1
shift
o=$1
shift

i=0
f=1
for scale
do
  perlin_bin=perlin_grad_`printf %04d $width`_`printf %04d $height`_`printf %.5e $scale`_`printf %04d $i`_000.bin
  #perlin_ppm=perlin_grad_`printf %04d $width`_`printf %04d $height`_`printf %.5e $scale`_`printf %04d $i`_000.ppm
  #perlin_png=perlin_grad_`printf %04d $width`_`printf %04d $height`_`printf %.5e $scale`_`printf %04d $i`_000.png
  perlin_contrast_bin=perlin_grad_`printf %04d $width`_`printf %04d $height`_`printf %.5e $scale`_`printf %04d $i`_000_c.bin
  #perlin_contrast_ppm=perlin_grad_`printf %04d $width`_`printf %04d $height`_`printf %.5e $scale`_`printf %04d $i`_000_c.ppm
  #perlin_contrast_png=perlin_grad_`printf %04d $width`_`printf %04d $height`_`printf %.5e $scale`_`printf %04d $i`_000_c.png
  #echo $perlin_bin
  #echo $perlin_ppm
  #echo $perlin_bin
  #echo $perlin_contrast_bin
  #echo $perlin_contrast_ppm
  #echo $perlin_contrast_png

  ./perlin_grad -w $width -h $height -c 1 -s $i -e $scale -o perlin_grad
  #./bin2ppm_sign -i $perlin_bin -o $perlin_ppm
  #convert $perlin_ppm $perlin_png
  #./bin2ppm_sign -i $perlin_contrast_bin -o $perlin_contrast_ppm
  #convert $perlin_contrast_ppm $perlin_contrast_png

  if [ $i -gt 0 ]
  then
    I=$i
    let "I=I+1"
    F=`printf %0${I}d $f`
    ./add_bin -a $o.bin -b $perlin_contrast_bin \
      -m 1.0 -n ${F:0:1}.${F:1} -o tmp.bin
    rm $o.bin
    mv tmp.bin $o.bin
  else
    cp $perlin_contrast_bin $o.bin
  fi
  rm $perlin_bin
  rm $perlin_contrast_bin
  let "i=i+1"
  let "f=f*5"
done
#./remap_bin -i $o.bin -o ${o}_c.bin \
#  `./bin_range -i $o.bin` -t '-1.0' -u 1.0
./bin2ppm_sign -i $o.bin -o $o.ppm
convert $o.ppm $o.png
#./bin2ppm_sign -i ${o}_c.bin -o ${o}_c.ppm
#convert ${o}_c.ppm ${o}_c.png
