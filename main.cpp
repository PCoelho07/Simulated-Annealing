#include <iostream>
// #include "Dgraph.hpp"
#include "sa.hpp"


int main(int argc, char const *argv[]){	
	Dgraph *d_graph = new Dgraph(); // Vazamento!! 
	if (argc >= 7 ) {
		d_graph->readFile(argv[1]);	
		d_graph->mountDG();

		// SimulatedAnnealing *sa = new SimulatedAnnealing(argv[2], atof(argv[3]), argv[4], atoi(argv[5]), argv[6], atof(argv[7]), d_graph);

		delete d_graph;
		// delete sa;
	}
	else
		std::cout << "Quantidade invalida de parâmetros!! \n";

	return 0;
}