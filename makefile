main: main.cc
	g++ $< -o $@ -lm

rand_gray: rand_gray.cc grid.h random.h
	g++ $< -o $@ -Icxxopts/include

rand_gray_1024.bin: rand_gray
	./rand_gray -w 1024 -h 1024 -o rand_gray_1024 -t 1

bin2ppm_gray: bin2ppm_gray.cc grid.h
	g++ $< -o $@ -Icxxopts/include

rand_gray_1024.ppm: rand_gray_1024.bin bin2ppm_gray
	./bin2ppm_gray -i $< -o $@

rand_gray_1024.png: rand_gray_1024.ppm
	convert $< $@

rand_vec: rand_vec.cc grid.h random.h
	g++ $< -o $@ -Icxxopts/include

rand_vec_1024_2.bin: rand_vec
	./rand_vec -w 1024 -h 1024 -d 2 -o rand_vec_1024_2 -t 1

bin2ppm_vec: bin2ppm_vec.cc grid.h
	g++ $< -o $@ -Icxxopts/include

rand_vec_1024_2.ppm: rand_vec_1024_2.bin bin2ppm_vec
	./bin2ppm_vec -i $< -o $@

rand_vec_1024_2.png: rand_vec_1024_2.ppm
	convert $< $@

rand_dir: rand_dir.cc grid.h random.h
	g++ $< -o $@ -Icxxopts/include

rand_dir_1024_2.bin: rand_dir
	./rand_dir -w 1024 -h 1024 -d 2 -o rand_dir_1024_2 -t 1

bin2ppm_dir: bin2ppm_dir.cc grid.h
	g++ $< -o $@ -Icxxopts/include

rand_dir_1024_2.ppm: rand_dir_1024_2.bin bin2ppm_dir
	./bin2ppm_dir -i $< -o $@

rand_dir_1024_2.png: rand_dir_1024_2.ppm
	convert $< $@

rand_col: rand_col.cc grid.h random.h
	g++ $< -o $@ -Icxxopts/include

rand_col_1024.bin: rand_col
	./rand_col -w 1024 -h 1024 -o rand_col_1024 -t 1

bin2ppm_col: bin2ppm_col.cc grid.h
	g++ $< -o $@ -Icxxopts/include

rand_col_1024.ppm: rand_col_1024.bin bin2ppm_col
	./bin2ppm_col -i $< -o $@

rand_col_1024.png: rand_col_1024.ppm
	convert $< $@

perlin_val: perlin_val.cc grid.h random.h noise.h
	g++ $< -o $@ -Icxxopts/include

perlin_val_1024_256.bin: perlin_val
	./perlin_val -w 1024 -h 1024 -c 256 -o perlin_val_1024_256 -t 1

perlin_val_1024_256.ppm: perlin_val_1024_256.bin bin2ppm_gray
	./bin2ppm_gray -i $< -o $@

perlin_val_1024_256.png: perlin_val_1024_256.ppm
	convert $< $@

perlin_grad: perlin_grad.cc grid.h random.h noise.h
	g++ $< -o $@ -Icxxopts/include

perlin_grad_0512_128.bin: perlin_grad
	./perlin_grad -w 512 -h 512 -c 128 -o perlin_grad_0512_128 -t 1

perlin_grad_0512_128.ppm: perlin_grad_0512_128.bin bin2ppm_gray
	./bin2ppm_gray -i $< -o $@

perlin_grad_0512_128.png: perlin_grad_0512_128.ppm
	convert $< $@

perlin_grad_1024_256.bin: perlin_grad
	./perlin_grad -w 1024 -h 1024 -c 256 -o perlin_grad_1024_256 -t 1

perlin_grad_1024_256.ppm: perlin_grad_1024_256.bin bin2ppm_gray
	./bin2ppm_gray -i $< -o $@

perlin_grad_1024_256.png: perlin_grad_1024_256.ppm
	convert $< $@

all: rand_gray_1024.png rand_vec_1024_2.png rand_dir_1024_2.png rand_col_1024.png perlin_val_1024_256.png perlin_grad_1024_256.png

clean:
	rm rand_gray
	rm rand_vec
	rm rand_dir
	rm rand_col
	rm bin2ppm_gray
	rm bin2ppm_vec
	rm bin2ppm_col
	rm perlin_val
	rm perlin_grad
	rm *.ppm
	rm *.bin
	rm *.png