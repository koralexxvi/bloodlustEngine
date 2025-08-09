if [ -f ./build/CMakeCache.txt ]; then
	rm ./build/CMakeCache.txt
fi

# note: glm is still using deprecated cmake 3.10
cmake -G Ninja -S . -B ./build -DCMAKE_POLICY_VERSION_MINIMUM=3.10
