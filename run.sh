#!/bin/bash

#make main
#./main
#convert fractal_perlin.ppm fractal_perlin.png
#convert fractal_perlin_path.ppm fractal_perlin_path.png
#convert fractal_perlin_path.png fractal_perlin_dx.ppm +append fractal_perlin_top.png
#convert fractal_perlin_normal.ppm fractal_perlin_dy.ppm +append fractal_perlin_bottom.png
#convert fractal_perlin_top.png fractal_perlin_bottom.png -append fractal_perlin_diff.png
s=1024
make rand_gray || exit
./rand_gray $s $s rand_gray_`printf %03d $s` 1 || exit
make bin2ppm_gray || exit
./bin2ppm_gray rand_gray_`printf %03d $s`.bin rand_gray_`printf %03d $s`.ppm || exit
convert rand_gray_`printf %03d $s`.ppm rand_gray_`printf %03d $s`.png
make rand_col || exit
./rand_col $s $s rand_col_`printf %03d $s` 1 || exit
make bin2ppm_col || exit
./bin2ppm_col rand_col_`printf %03d $s`.bin rand_col_`printf %03d $s`.ppm || exit
convert rand_col_`printf %03d $s`.ppm rand_col_`printf %03d $s`.png
d=2
make rand_vec || exit
./rand_vec $s $s $d rand_vec_`printf %03d $s`_$d 1 || exit
make bin2ppm_vec || exit
./bin2ppm_vec rand_vec_`printf %03d $s`_$d.bin rand_vec_`printf %03d $s`_$d.ppm || exit
convert rand_vec_`printf %03d $s`_$d.ppm rand_vec_`printf %03d $s`_$d.png
make rand_dir || exit
./rand_dir $s $s $d rand_dir_`printf %03d $s`_$d 1 || exit
make bin2ppm_dir || exit
./bin2ppm_dir rand_dir_`printf %03d $s`_$d.bin rand_dir_`printf %03d $s`_$d.ppm || exit
convert rand_dir_`printf %03d $s`_$d.ppm rand_dir_`printf %03d $s`_$d.png