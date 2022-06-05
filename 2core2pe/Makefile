all : main.cpp bootstrap.S
	riscv32-unknown-elf-g++ -std=c++14 main.cpp bootstrap.S -o main -nostartfiles -march=rv32ima -mabi=ilp32
	
sim: all
	/home/user/ee6470/riscv-vp/vp/build/bin/riscv-vp-acc-2core --intercept-syscalls main
	
clean:
	rm -f main
