g++ --std=c++17 -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/darwin/ compress_bc7.cpp -o bc7-compressor.o -O3 && g++ -shared -fPIC -o bc7-compressor.dylib bc7-compressor.o -lc
