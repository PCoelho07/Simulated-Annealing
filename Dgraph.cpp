#include "Dgraph.hpp"


Dgraph::Dgraph(){
	this->jobs = 0;
	this->mach = 0;
}

void Dgraph::readFile(const char *file){
	cout << "\n \n Iniciando leitura de arquivo ...\n";
	string linha;
	unsigned aux;
	vector<int> vetor;
	ifstream f(file);	
	
	// /*Número de trabalhos*/	
	f >> aux; 
	this->jobs = aux;

	// /*Número de máquinas*/
	f >> aux;
	this->mach = aux;

	while(true){
		double value;
		if(!(f >> value))
			break;
		vetor.push_back(value);
	}

	this->tamanho = vetor.size();

	this->dados = new int*[vetor.size()];
	for(int i =0; i < vetor.size(); i++){
		this->dados[i] = new int[vetor.size()];
	}

	int cont = 0;
	for(int i =0; i < this->jobs; i++){
		for(int j =0; j < this->mach*2; j++){
			this->dados[i][j] = vetor.at(cont);
			cont++;			
		}
	}
	// /*
	// 	Exibição(this->dados)
	// */
	for(int i =0; i < this->jobs; i++){
		for(int j =0; j < this->mach*2; j++){
			cout << dados[i][j] << " ";
		}
		cout << endl;
	}

	if(vetor.size() != this->jobs*(this->mach*2))
		cout << "Erro durante leitura!!\n";
	
	cout << "\nJobs: " << this->jobs << " Machines: " << this->mach;
	// cout << "\nMatrix size: " << this->jobs*(this->mach*2) << "\n";

	cout << "\n\n";

	cout << "\n \n Leitura concluída!\n";

	// this->mountDG(); // Constrói o grafo
}


void Dgraph::mountDG(){	
	Task *t;	
	/*
		Vértices do grafo
	*/
	t = new Task(0, -1, -1, -1); //Source
	this->task_list.push_back(t);

	int index = 1;
	for(int i = 0; i < this->jobs; i++){
		for(int j = 0; j <  this->mach*2; j+=2){
			// cout << index<< endl;
			t = new Task(index, i, this->dados[i][j], this->dados[i][j+1]);
			this->task_list.push_back(t);
			index++;
		}
	}
	// delete [] t;
	t = new Task(101, -1, -1, -1);
	this->task_list.push_back(t); //Target

	cout << this->task_list.size() << endl; 

	// for(int i =0; i < this->task_list.size(); i++){
	// 	cout << this->task_list.at(i)->id_task << endl;
	// }

	/*
		Monta o grafo sem arestas disjuntivas, ou seja, obedecendo apenas a precedência de operações
	*/
	this->adj = new vector<Task*>[this->task_list.size()];

	for(int i=0; i<this->jobs; i++){
		this->addEdge(this->task_list.at(0), this->task_list.at((i*this->mach)+1));
	}


	for(int i=1; i < this->task_list.size()-1; i++){
		if(this->task_list.at(i)->job_id == this->task_list.at(i+1)->job_id){
			//this->adj[i].push_back(this->task_list.at(i+1));
			this->addEdge(this->task_list.at(i), this->task_list.at(i+1));
		}
		else{
			this->addEdge(this->task_list.at(i), this->task_list.at(this->task_list.size()-1));
		}
	}
	// t = new Task(100, -1, -1, -1);
	//this->adj[this->task_list.size()-1].push_back(*t); 
	// this->addEdge(this->task_list.at(this->task_list.size()-2), this->task_list.at(this->task_list.size()-1)); //último elemento apontando para o 'Target'

	// Exibição
	for(int i = 0; i < this->task_list.size(); i++){
		cout << "Vertex: " << this->task_list.at(i)->id_task << " - ";
		if(this->adj[this->task_list.at(i)->id_task].size() > 0) {
			for(int j =0; j < this->adj[this->task_list.at(i)->id_task].size(); j++){
				cout << "Adjacents: " << this->adj[i].at(j)->id_task << "\n";
			}			
		}
		else {
			cout << "Não há adjacentes!!!\n";
		}
	}
	// delete t;
}


/*
	Adiciona v2 à lista de vértices adjacentes de v1;
*/
bool Dgraph::addEdge(Task *v1, Task *v2){
	if(this->adj != NULL){
		this->adj[v1->id_task].push_back(v2);
		return true;
	}
	return false;
}

/*
	Remove a aresta entre v1 e v2
*/
void Dgraph::delEdge(Task *v1, Task *v2){
	if(this->adj != NULL) {
		vector<Task*>::iterator it = find(this->adj[v1->id_task].begin(), this->adj[v2->id_task].end(), v2);
		this->adj[v1->id_task].erase(it);
	}
}

void Dgraph::setTaskList(vector<Task*> taskList){
	this->task_list = taskList;
}

Dgraph::~Dgraph() {
	delete [] this->adj;

	for(int i=0; i<this->tamanho; i++)
		delete []this->dados;
}