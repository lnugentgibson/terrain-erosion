main: main.cc
	g++ $< -o $@ -lm

rand_gray: rand_gray.cc grid.h random.h

bin2ppm_gray: bin2ppm_gray.cc grid.h

rand_col: rand_col.cc grid.h random.h

bin2ppm_col: bin2ppm_col.cc grid.h

clean:
	rm *.ppm
	rm *.bin
	rm *.png
	rm *.txt