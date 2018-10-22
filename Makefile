all: compile_montador compile_ligador

compile_montador:
	g++ -std=c++11 -ansi -Wall -o montador montador.cpp

compile_ligador:
	g++ -std=c++11 -ansi -Wall -o ligador ligador.cpp

clear:
	rm ligador
	rm montador
	rm *.obj
	rm *.e
	rm *.pre