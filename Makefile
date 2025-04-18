all:
	cd ./stack             && make
	cd ./spu               && make
	cd ./assembler         && make
	cd ./front_end         && make
	cd ./back_end          && make
	cd ./reverse_front_end && make
	cd ./middle_end        && make

clean:
	cd ./stack             && make clean
	cd ./spu               && make clean
	cd ./assembler         && make clean
	cd ./front_end         && make clean
	cd ./back_end          && make clean
	cd ./reverse_front_end && make clean
	cd ./middle_end        && make clean