main: main.cc
	g++ $< -o $@ -lm

rand_gray: rand_gray.cc grid.h random.h
	g++ -std=c++17 $< -o $@ -Icxxopts/include

rand_gray_1024.bin: rand_gray
	./rand_gray -w 1024 -h 1024 -o $@ -t 1 --seed=-1

bin2ppm_gray: bin2ppm_gray.cc grid.h
	g++ -std=c++17 $< -o $@ -Icxxopts/include

rand_gray_1024.ppm: rand_gray_1024.bin bin2ppm_gray
	./bin2ppm_gray -i $< -o $@

rand_gray_1024.png: rand_gray_1024.ppm
	convert $< $@

rand_vec: rand_vec.cc grid.h random.h
	g++ -std=c++17 $< -o $@ -Icxxopts/include

rand_vec_1024_2.bin: rand_vec
	./rand_vec -w 1024 -h 1024 -d 2 -o $@ -t 1 --seed=-1

bin2ppm_vec: bin2ppm_vec.cc grid.h
	g++ -std=c++17 $< -o $@ -Icxxopts/include

rand_vec_1024_2.ppm: rand_vec_1024_2.bin bin2ppm_vec
	./bin2ppm_vec -i $< -o $@

rand_vec_1024_2.png: rand_vec_1024_2.ppm
	convert $< $@

rand_dir: rand_dir.cc grid.h random.h
	g++ -std=c++17 $< -o $@ -Icxxopts/include

rand_dir_1024_2.bin: rand_dir
	./rand_dir -w 1024 -h 1024 -d 2 -o $@ -t 1 --seed=-1

bin2ppm_dir: bin2ppm_dir.cc grid.h
	g++ -std=c++17 $< -o $@ -Icxxopts/include

rand_dir_1024_2.ppm: rand_dir_1024_2.bin bin2ppm_dir
	./bin2ppm_dir -i $< -o $@

rand_dir_1024_2.png: rand_dir_1024_2.ppm
	convert $< $@

rand_col: rand_col.cc grid.h random.h
	g++ -std=c++17 $< -o $@ -Icxxopts/include

rand_col_1024.bin: rand_col
	./rand_col -w 1024 -h 1024 -o $@ -t 1 --seed=-1

bin2ppm_col: bin2ppm_col.cc grid.h
	g++ -std=c++17 $< -o $@ -Icxxopts/include

rand_col_1024.ppm: rand_col_1024.bin bin2ppm_col
	./bin2ppm_col -i $< -o $@

rand_col_1024.png: rand_col_1024.ppm
	convert $< $@

perlin_val: perlin_val.cc grid.h random.h noise.h
	g++ -std=c++17 $< -o $@ -Icxxopts/include

perlin_val_1024_0256.bin: perlin_val
	./perlin_val -w 1024 -h 1024 -c 256 -o $@ -t 1

bin2ppm_sign: bin2ppm_sign.cc grid.h
	g++ -std=c++17 $< -o $@ -Icxxopts/include

perlin_val_1024_0256.ppm: perlin_val_1024_0256.bin bin2ppm_sign
	./bin2ppm_sign -i $< -o $@

perlin_val_1024_0256.png: perlin_val_1024_0256.ppm
	convert $< $@

perlin_grad: perlin_grad.cc grid.h random.h noise.h
	g++ -std=c++17 $< -o $@ -Icxxopts/include

perlin_grad_0512_0128.bin: perlin_grad
	./perlin_grad -w 512 -h 512 -c 128 -o $@ -t 1

perlin_grad_0512_0128.ppm: perlin_grad_0512_0128.bin bin2ppm_sign
	./bin2ppm_sign -i $< -o $@

perlin_grad_0512_0128.png: perlin_grad_0512_0128.ppm
	convert $< $@

scales := 1024 0512 0256 0128 0064 0032 0016 0008

perlin_grad_1024_1024.bin perlin_grad_1024_0512.bin perlin_grad_1024_0256.bin perlin_grad_1024_0128.bin perlin_grad_1024_0064.bin perlin_grad_1024_0032.bin perlin_grad_1024_0016.bin perlin_grad_1024_0008.bin : perlin_grad_1024_%.bin : perlin_grad
	./perlin_grad -w 1024 -h 1024 -c $* -o $@ -t 1

perlin_grad_1024_1024_c.bin perlin_grad_1024_0512_c.bin perlin_grad_1024_0256_c.bin perlin_grad_1024_0128_c.bin perlin_grad_1024_0064_c.bin perlin_grad_1024_0032_c.bin perlin_grad_1024_0016_c.bin perlin_grad_1024_0008_c.bin : perlin_grad_1024_%_c.bin : perlin_grad_1024_%.bin remap_bin bin_range
	./remap_bin -i $< -o $@ `./bin_range -i $<` -t '-1.0' -u 1.0

perlin_grad_1024_1024.ppm perlin_grad_1024_0512.ppm perlin_grad_1024_0256.ppm perlin_grad_1024_0128.ppm perlin_grad_1024_0064.ppm perlin_grad_1024_0032.ppm perlin_grad_1024_0016.ppm perlin_grad_1024_0008.ppm : perlin_grad_1024_%.ppm : perlin_grad_1024_%_c.bin bin2ppm_sign
	./bin2ppm_sign -i $< -o $@

perlin_grad_1024_1024.png perlin_grad_1024_0512.png perlin_grad_1024_0256.png perlin_grad_1024_0128.png perlin_grad_1024_0064.png perlin_grad_1024_0032.png perlin_grad_1024_0016.png perlin_grad_1024_0008.png : perlin_grad_1024_%.png : perlin_grad_1024_%.ppm
	convert $< $@

perlin_grad_1024: perlin_grad_1024_1024.png perlin_grad_1024_0512.png perlin_grad_1024_0256.png perlin_grad_1024_0128.png perlin_grad_1024_0064.png perlin_grad_1024_0032.png perlin_grad_1024_0016.png perlin_grad_1024_0008.png

add_bin: add_bin.cc binimg.h
	g++ -std=c++17 $< -o $@ -Icxxopts/include

perlin_grad_1024_0512_s.bin: perlin_grad_1024_1024_c.bin perlin_grad_1024_0512_c.bin add_bin
	add_bin -a perlin_grad_1024_1024_c.bin -b perlin_grad_1024_0512_c.bin -m 1.0 -n 0.5 -o $@

perlin_grad_1024_0256_s.bin: perlin_grad_1024_0512_s.bin perlin_grad_1024_0256_c.bin add_bin
	add_bin -a perlin_grad_1024_0512_s.bin -b perlin_grad_1024_0256_c.bin -m 1.0 -n 0.25 -o $@

perlin_grad_1024_0128_s.bin: perlin_grad_1024_0256_s.bin perlin_grad_1024_0128_c.bin add_bin
	add_bin -a perlin_grad_1024_0256_s.bin -b perlin_grad_1024_0128_c.bin -m 1.0 -n 0.125 -o $@

perlin_grad_1024_0064_s.bin: perlin_grad_1024_0128_s.bin perlin_grad_1024_0064_c.bin add_bin
	add_bin -a perlin_grad_1024_0128_s.bin -b perlin_grad_1024_0064_c.bin -m 1.0 -n 0.0625 -o $@

perlin_grad_1024_0032_s.bin: perlin_grad_1024_0064_s.bin perlin_grad_1024_0032_c.bin add_bin
	add_bin -a perlin_grad_1024_0064_s.bin -b perlin_grad_1024_0032_c.bin -m 1.0 -n 0.03125 -o $@

perlin_grad_1024_0016_s.bin: perlin_grad_1024_0032_s.bin perlin_grad_1024_0016_c.bin add_bin
	add_bin -a perlin_grad_1024_0032_s.bin -b perlin_grad_1024_0016_c.bin -m 1.0 -n 0.015625 -o $@

perlin_grad_1024_0008_s.bin: perlin_grad_1024_0016_s.bin perlin_grad_1024_0008_c.bin add_bin
	add_bin -a perlin_grad_1024_0016_s.bin -b perlin_grad_1024_0008_c.bin -m 1.0 -n 0.0078125 -o $@

perlin_grad_1024_0008_s_c.bin: perlin_grad_1024_0008_s.bin remap_bin bin_range
	./remap_bin -i $< -o $@ `./bin_range -i $<` -t '-1.0' -u 1.0

perlin_grad_1024_0008_s_c.ppm: perlin_grad_1024_0008_s_c.bin bin2ppm_sign
	./bin2ppm_sign -i $< -o $@

perlin_grad_1024_0008_s_c.png: perlin_grad_1024_0008_s_c.ppm
	convert $< $@

bin_range: bin_range.cc binimg.h
	g++ -std=c++17 $< -o $@ -Icxxopts/include

remap_bin: remap_bin.cc binimg.h
	g++ -std=c++17 $< -o $@ -Icxxopts/include

perlin_grad_1024_1024_c.ppm: perlin_grad_1024_1024_c.bin bin2ppm_sign
	./bin2ppm_sign -i $< -o $@

perlin_grad_1024_1024_c.png: perlin_grad_1024_1024_c.ppm
	convert $< $@

all: rand_gray_1024.png rand_vec_1024_2.png rand_dir_1024_2.png rand_col_1024.png perlin_val_1024_0256.png perlin_grad_1024

clean: clean_images

clean_binaries:
	rm rand_gray
	rm rand_vec
	rm rand_dir
	rm rand_col
	rm bin2ppm_gray
	rm bin2ppm_vec
	rm bin2ppm_dir
	rm bin2ppm_col
	rm bin2ppm_sign
	rm perlin_val
	rm perlin_grad
	rm add_bin
	rm bin_range
	rm remap_bin

clean_images:
	rm *.ppm
	rm *.bin
	rm *.png