#include "Edge.hpp"

Edge::Edge(Task *src, Task *trgt){
	this->source = src;
	this->target = trgt;
	this->directed = true;
	this->conjunctive = true;
}

void Edge::setDirected(bool isDir){
	this->directed = isDir;
}

void Edge::setConjunctive(bool isConj){
	this->conjunctive = isConj;
}