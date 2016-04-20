all:
	flex -o sdf_lexer.gen.cpp src/sdf_lexer.l
	bison -r all -o sdf_parser.gen.cpp src/sdf_parser.y
	g++ -g -std=c++11 -I. -Isrc -c sdf_lexer.gen.cpp
	g++ -g -std=c++11 -I. -Isrc -c sdf_parser.gen.cpp
	g++ -g -std=c++11 -I. -Isrc -c src/sdf_loader.cpp
	g++ -g -std=c++11 -I. -Isrc -c src/main.cpp
	g++ -g -std=c++11 sdf_loader.o sdf_parser.gen.o sdf_lexer.gen.o main.o -o sdf_parse

clean:
	rm -f *.gen.hpp *.gen.cpp stack.hh *.o sdf_parse
