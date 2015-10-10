#ifndef __SIMULATED_ANNEALING_HPP
#define __SIMULATED_ANNEALING_HPP
#endif

#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "Dgraph.hpp"

using namespace std;

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
		float calculaCusto(Dgraph*);	
		void mostraSolucao();
};