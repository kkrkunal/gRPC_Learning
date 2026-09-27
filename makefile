.PHONY: all server srun client crun clean

all:
	cmake -B build
	cmake --build build -j$(nproc)

server:
	cmake -B build
	cmake --build build --target server

srun:
	./build/server

client:
	cmake -B build
	cmake --build build --target client

crun:
	./build/client

clean:
	rm -rf build