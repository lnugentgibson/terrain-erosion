#!/bin/bash

#make main
#./main
#convert fractal_perlin.ppm fractal_perlin.png
#convert fractal_perlin_path.ppm fractal_perlin_path.png
#convert fractal_perlin_path.png fractal_perlin_dx.ppm +append fractal_perlin_top.png
#convert fractal_perlin_normal.ppm fractal_perlin_dy.ppm +append fractal_perlin_bottom.png
#convert fractal_perlin_top.png fractal_perlin_bottom.png -append fractal_perlin_diff.png
s=256
make rand_gray
./rand_gray $s $s rand_gray_`printf %03d $s` 1
make bin2ppm_gray
./bin2ppm_gray rand_gray_`printf %03d $s`.bin rand_gray_`printf %03d $s`.ppm
convert rand_gray_`printf %03d $s`.ppm rand_gray_`printf %03d $s`.png
make rand_col
./rand_col $s $s rand_col_`printf %03d $s` 1
make bin2ppm_col
./bin2ppm_col rand_col_`printf %03d $s`.bin rand_col_`printf %03d $s`.ppm
convert rand_col_`printf %03d $s`.ppm rand_col_`printf %03d $s`.png