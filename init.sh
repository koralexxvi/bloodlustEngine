if [ -f ./build/CMakeCache.txt ]; then
	rm ./build/CMakeCache.txt
fi
cmake -S . -B ./build
