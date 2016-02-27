main: src/main.cpp src/analyzer.cpp src/analyzer.hpp src/database_connection.cpp src/database_connection.hpp src/file_reader.cpp src/file_reader.hpp src/user.hpp src/device.hpp src/login.hpp
	g++-4.9 src/main.cpp -std=c++11 -o bin/main -pthread -lmongoclient -lboost_thread -lboost_filesystem -lboost_program_options -lboost_system -lboost_regex

clean:
	rm bin/main
