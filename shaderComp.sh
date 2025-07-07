if [ ! -d ./res/shader/compiled ]; then
	mkdir ./res/shader/compiled
fi
cd ./res/shader/compiled
glslc -c ../src/*
