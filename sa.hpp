#ifndef __SIMULATED_ANNEALING_HPP
#define __SIMULATED_ANNEALING_HPP
#endif
#define INFNEG -1.0


#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <stack>
// #include <iostream>
#include "Dgraph.hpp"

using namespace std;

struct Arv
{
	Task *pai;
};


class SimulatedAnnealing{
	private:
		float **custo; //Matriz de custo
		int *melhor; //grafo de melhor caminho
		int custoMelhor; // Custo do melhor caminho
		int tamanho; // tamanho do grafo

	public:
		// --t0 1000 --nit 20 --txalpha 0.99 
		SimulatedAnnealing(const char*, double, const char*, int, const char*, float);
		SimulatedAnnealing();
		~SimulatedAnnealing();
	private:
		void lerArq(const char*);
	public:
		void solucaoInicial(Dgraph*);
		void solucaoVizinha(int*, int*);
		int calculaCusto(Dgraph*);	
		void mostraSolucao();
};