all:
	flex -o sdf_lexer.gen.cpp src/sdf_lexer.l
	g++ -std=c++11 -Isrc -c sdf_lexer.gen.cpp
	g++ -std=c++11 -Isrc -c src/main.cpp
	g++ -std=c++11 sdf_lexer.gen.o main.o -o sdf_parse

clean:
	rm -f sdf_lexer.gen.cpp *.o sdf_parse
