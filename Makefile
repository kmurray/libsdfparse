all:
	flex -o sdf_scanner.gen.cpp src/sdf_scanner.l
	g++ -std=c++11 -Isrc -c sdf_scanner.gen.cpp
	g++ -std=c++11 -Isrc -c src/main.cpp
	g++ -std=c++11 sdf_scanner.gen.o main.o -o sdf_parse

clean:
	rm -f sdf_scanner.gen.cpp *.o sdf_parse
