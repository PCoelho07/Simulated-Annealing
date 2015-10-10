#ifndef __EDGE_HPP__
#define __EDGE_HPP__

#include "task.cpp"

class Edge{
	private:
		Task *source;
		Task *target;
		int peso; // O valor do tempo de processamento do 'source'.
		bool directed;
		bool conjunctive; // Se 'false', a aresta é disjuntiva.


	public:
		Edge(Task*, Task*);
		~Edge();
		bool isDirected(){ return this->directed; }
		bool isConjunctive(){ return this->conjunctive; }
		void setDirected(bool);
		void setConjunctive(bool);	
		void setSrc(Task);
		void setTrgt(Task);
};


#endif