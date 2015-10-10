#include <iostream>
#include "Dgraph.hpp"
#include "sa.hpp"


int main(int argc, char const *argv[]){	
	Dgraph *d_graph = new Dgraph();
	d_graph->readFile(argv[1]);	
	d_graph->mountDG();

	
	SimulatedAnnealing *sa = new SimulatedAnnealing();
	sa->solucaoInicial(d_graph);

	return 0;
}