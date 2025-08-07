if [ -f ./build/CMakeCache.txt ]; then
	rm ./build/CMakeCache.txt
fi

# note: glm is still using deprecated cmake 3.10
cmake -S . -B ./build -DCMAKE_POLICY_VERSION_MINIMUM=3.10

if [ ! -s ./lib/glm ]; then
	cd lib
	ln -rs glm-src/glm glm
fi
