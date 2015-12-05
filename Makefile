main: src/main.cpp src/analyzer.cpp src/analyzer.hpp src/database_connection.cpp src/database_connection.hpp src/file_reader.cpp src/file_reader.hpp
	g++ src/main.cpp -std=c++11 -o bin/main

clean:
	rm bin/main
