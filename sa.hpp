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
#include "Dgraph.hpp"

using namespace std;

struct makespan{
	int custo; // Custo do caminho crítico
	stack<Task*> nodos; // Nodos que compõe o caminho crítico
	int *dists; //vetor que contém a distância do maior caminho da fonte para todos os nós;
};


class SimulatedAnnealing{
	private:
		float **custo; //Matriz de custo
		makespan melhor; //grafo de melhor caminho
		int tamanho; // tamanho do grafo
		vector<Task*> *m;
		vector<Task*> tl;
		int *prev;

	public:
		// --t0 1000 --nit 20 --txalpha 0.99 Dgraph
		SimulatedAnnealing(const char*, double, const char*, int, const char*, float,  Dgraph*);
		// SimulatedAnnealing();
		~SimulatedAnnealing();
	private:
		void lerArq(const char*);
	public:
		void solucaoInicial(Dgraph*);
		void solucaoVizinha(Dgraph*, makespan);
		makespan calculaCusto(Dgraph*, list<Task*>*);	
		void mostraSolucao();
		Task* getById(int);
		stack<Task*> stack_up(int*, int);
		bool isInPath(int, vector<Task*>);
		void swapTask(Dgraph*, vector<Task*>*, int, int);
		bool checkSucessiveMachine(Dgraph*, int, int, int);
};