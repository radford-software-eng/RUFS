all: RATCH

RATCH: RATCH.cpp FShandler.cpp FileSystem.hpp
	clang++ -Wall -std=c++17 RATCH.cpp FShandler.cpp -Ofast -o RATCH

rucs: RATCH.cpp FShandler.cpp FileSystem.hpp
	clang++ -Wall RATCH.cpp FShandler.cpp -std=c++17 -stdlib=libc++ -Ofast -o RATCH

gcc: RATCH.cpp FShandler.cpp FileSystem.hpp
	g++ -o RUFS RATCH.cpp FShandler.cpp

clean:
	rm RATCH