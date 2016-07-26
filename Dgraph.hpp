#ifndef __DGRAPH_HPP__
#define __DGRAPH_HPP__

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstring>
// #include "Edge.hpp"
#include "task.cpp"

using namespace std;

class Dgraph{
	private:
		vector<Task*> task_list; // Nodos do grafo;
		vector<Task*> *adj; //Vetor de listas;
		int **dados; //Dados vindos à partir do arquivo(bruto)
		int jobs; //Número de jobs
		int mach; //Número de máquinas
		int tamanho;

	public:
		Dgraph();
		~Dgraph();
		void readFile(const char*); //Ler o arquivo
		void mountDG(); // Monta o grafo com os arcos direcionados entre operações que possuem precedência direta.	
		vector<Task*> *getGraph(){ return this->adj; };
		vector<Task*> getTaskList(){ return this->task_list; };
		void setTaskList(vector<Task*>);
		bool addEdge(Task *v1, Task *v2); // Adiciona arestas no grafo
		void delEdge(Task *v1, Task *v2); //Remove uma aresta no grafo
		int getJobs(){return this->jobs;};
		int getMach(){return this->mach;};
};


#endif