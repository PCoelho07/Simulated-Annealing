#include "sa.hpp"

SimulatedAnnealing::SimulatedAnnealing(const char* strT0, double t0, const char* strNmax, int nmax, const char* txredux, float talpha){
	

	// this->melhor = NULL;
	// int *corrente = new int[this->tamanho]; //Solução corrente
	// int *vizinho = new int[this->tamanho]; //Solução vizinha
	// //int t0; //Temperatura inicial
	// double tf = 0; // Temperatura final
	// double t = t0; // Temṕeratura corrente
	// double txReduc = talpha; // Taxa de redução da temperatura
	// int custoCorrente =0, custoVizinho =0;
	// // int nmax = 0;
	// int diferenca =0;

	// this->melhor = this->solucaoInicial();
	// memcpy(corrente, this->melhor, sizeof(int)*this->tamanho);
	// this->custoMelhor = custoCorrente = this->calculaCusto(this->melhor);
	// double a =0, p =0;
	// int aux =0;

	// int btmax = 100, melhorInt =0;
	// srand(time(NULL));
	// 	while(t > 0.0001){
	// 		++aux;
	// 		for(int contador =0; contador < nmax; ++contador){
	// 			this->solucaoVizinha(corrente, vizinho);
	// 			custoVizinho = this->calculaCusto(vizinho);
	// 			diferenca = custoVizinho - custoCorrente;
	// 			a =double(double(rand())/double(RAND_MAX));
	// 			p =exp(double(-diferenca/t));
	// 				if(diferenca < 0 || a < p){
	// 					memcpy(corrente, vizinho, sizeof(int)*this->tamanho);
	// 					custoCorrente = custoVizinho;
	// 					if(custoCorrente < this->custoMelhor){
	// 						memcpy(this->melhor, corrente, sizeof(int)*this->tamanho);
	// 						this->custoMelhor = custoCorrente;
	// 						melhorInt=aux;
	// 					}
	// 				}
	// 		}
	// 		t = txReduc*t; // reduz a temperatura
	// 		if(aux % 200 == 0){
	// 			nmax += 10;
	// 		}
	
	// 	}

	// // this->mostraSolucao(); //Mostra solução
	// cout << this->calculaCusto(this->melhor) << "\n";
	// delete[] corrente;
	// delete[] vizinho;
}

SimulatedAnnealing::SimulatedAnnealing(){

}

/*
	Coleta e estrutura os dados vindo do arquivo.
*/
void SimulatedAnnealing::lerArq(const char *nomeArq){
	char nome[40];
	char tipo[20]; // TYPE
	char formato[20]; // EDGE_WEIGHT_TYPE
	char flag[255];

	FILE *f = fopen(nomeArq, "r");

	if(f == NULL){
		cout << "\nArquivo nao existe!!\n";
		int i=0;
		while(nomeArq[i] != '\0'){
			cout << nomeArq[i];
			i++;
		}
		exit(0);
	}

	/* 
		Captura apenas o cabeçalho do arquivo 
		Obs.: Adaptado do código do prof. Givanaldo
	*/

	struct ponto {double x, y; } *p;
	while (!feof(f)){
		fscanf(f, "%s", flag);
		if(strcmp(flag, "NAME:") == 0)
			fscanf(f, " %s", nome);
		else if(strcmp(flag, "NAME") == 0)
			fscanf(f, " : %s", nome);
		else if (strcmp(flag, "DIMENSION:") == 0)
			fscanf(f, " %d", &this->tamanho);
		else if (strcmp(flag, "DIMENSION") == 0)
			fscanf(f, " : %d", &this->tamanho);
		else if (strcmp(flag, "EDGE_WEIGHT_TYPE:") == 0)
			fscanf(f, " %s", formato);
		else if (strcmp(flag, "EDGE_WEIGHT_TYPE") == 0)
			fscanf(f, " : %s", formato);
		else if (strcmp(flag, "TYPE:") == 0)
			fscanf(f, " %s", tipo);
		else if (strcmp(flag, "TYPE") == 0)
			fscanf(f, " : %s", tipo);
		else if (strcmp(flag, "NODE_COORD_SECTION") == 0)
			break;
		else if (strcmp(flag, "EOF") == 0) {
			printf("ERRO: O campo EDGE_WEIGHT_FORMAT ou NODE_COORD_SECTION nao existe no arquivo %s.\n", nomeArq);
			exit(0);
		}
	}

	p = new ponto[this->tamanho];
	double tmp;
	for(int i=0; i<this->tamanho; i++)
		fscanf(f, "%lf %lf %lf\n", &tmp, &(p[i].x), &(p[i].y));

	this->custo = new float*[this->tamanho];
	for(int x =0; x < this->tamanho; ++x){
		this->custo[x] = new float[this->tamanho];
	}

	/* Alimento a matriz de distancias entre as cidades */
	for(int i = 0; i < this->tamanho; i++){
		for(int j = 0; j < this->tamanho; j++){
			if(i == j)
				custo[i][j] = 0;
			else
				custo[i][j] = (int) nearbyint(sqrt(pow(p[i].x - p[j].x, 2) + pow(p[i].y - p[j].y, 2)));			
			custo[j][i] = custo[i][j];
		}
	}
	delete[] p;
	fclose(f);
}

/*
	Algoritmo de Khan
*/
vector<Task*> ordTop(Dgraph *d_graph){
	vector<Task*> vertex = d_graph->getTaskList();
	vector<Task*> *graph = d_graph->getGraph();
	vector<Task*> s; // Lista de vertices sem precedência.
	vector<Task*> l; // Lista ordenada topologicamente.

	s.push_back(vertex.at(0));
	vector<Task*>::iterator it = l.begin();

	while(s.size() != 0){
		l.push_back(*(s.begin())); //Adiciono a l o primeiro vértice de s
		s.erase(s.begin()); //Removo o primeiro elemento de S
		
		for(int i =0; i < graph[(*it)->id_task].size(); i++) //Para cada vertice adjacente ao primeiro da lista 'l', adiciona a lista 's';
			s.push_back(graph[(*it)->id_task].at(i));
	}

	if(l.size() != vertex.size()){
		cout << "\nOcorreu um ERRO!\n"; // O grafo possui um ciclo
		exit(EXIT_FAILURE);
	}

	return l;
}


/*
	Dado um grafo disjuntivo, calcula o makespan e seu custo
*/
 makespan SimulatedAnnealing::calculaCusto(Dgraph *d_graph){
	vector<Task*> ord = ordTop(d_graph); //Ordenação topológica;
	vector<Task*> *graph = d_graph->getGraph();
	int *dists = new int[d_graph->getTaskList().size()];
	stack<Task*> nodes_cp; // Pilha de elementos do caminho crítico

	dists[d_graph->getTaskList().at(0)->id_task] = 0;
	for(int i=1; i < d_graph->getTaskList().size(); i++){
		dists[d_graph->getTaskList().at(i)->id_task] = INFNEG;
	}

	vector<Task*>::iterator v; 
	for(v = ord.begin(); v != ord.end(); v++){
		for(int i= 0; i<graph[(*v)->id_task].size(); i++){
			if(dists[graph[(*v)->id_task].at(i)->id_task] < (dists[(*v)->id_task] + graph[(*v)->id_task].at(i)->duration)){
				dists[graph[(*v)->id_task].at(i)->id_task] = dists[(*v)->id_task] + graph[(*v)->id_task].at(i)->duration;
				graph[(*v)->id_task].at(i)->pai = (*v); 
			}
		}
	}

	vector<Task*>::iterator t = d_graph->getTaskList().end();
	t--;
	nodes_cp.push((*t));
	while(*t != 0){
		if((*t)->pai){
			nodes_cp.push((*t)->pai);
			(*t) = (*t)->pai;
		}
		else
			(*t) = 0;
	}
	makespan R;
	R.custo = dists[101];
	R.nodos = nodes_cp;
	R.dists = dists;

	return R; //Estrutura que contém o custo e os nodos do caminho crítico;
	
	/*
		Pela propriedade de relaxamento de caminho(Lema 24.15 - Livro Cormen), se existir um caminho do 's'(source) e 't'(target),
		dists[t] = minpath(s, t), nesse caso, dists[t] = maxpath(s, t).
	*/
}

/*
	Regra shortest path time
*/
void SimulatedAnnealing::solucaoInicial(Dgraph *d_graph){
	int qtdMach = d_graph->getMach();
	vector<Task*> *graph = d_graph->getGraph();
	this->m = new vector<Task>[qtdMach];
	vector<Task*> *m0 = new vector<Task*>[qtdMach];
	
	for(int i =0; i < qtdMach; i++){	
		for(int j = 0; j < d_graph->getTaskList().size(); j++){
			if(d_graph->getTaskList().at(j)->machine_id == i)
				this->m[i].push_back((*d_graph->getTaskList().at(j)));
		}
	}

	//Para cada máquina, executa SPT
	for(int i = 0; i < qtdMach; i++){
		sort(this->m[i].begin(), this->m[i].end()); // O(N*log n)
	}

	//Adiciona as arestas no grafo
	for(int i =0; i < qtdMach; i++){
		for(int j=0; j < this->m[i].size()-1; j++){
			d_graph->addEdge(&(this->m[i][j]), &(this->m[i][j+1]));
		}
	}
}

/*
	
Verifica se todas as operações entre i e j no caminho crítico, são da mesma máquina (Def de par crítico)
	Supondo que i e j são da mesma máquina

*/

bool checkMachine(vector<Task*> v, int i, int j, int idMachine){
	vector<Task*>::iterator x = find(v.begin(), v.end(), v.at(i));
	vector<Task*>::iterator y = find(v.begin(), v.end(), v.at(j));
	vector<Task*>::iterator it;

	for(it = x; it != y; it++){
		if((*it)->machine_id != idMachine)
			return false;
	}
	return true;
}

/*
	Gera uma solução vizinha à que foi passada na função.
*/
void SimulatedAnnealing::solucaoVizinha(Dgraph *d_graph, makespan R){
	vector<Task*> t = d_graph->getTaskList();
	int custo = R.custo;
	stack<Task*> nodo = R.nodos;
	vector<Task*> v;
	int id_mach;

	while(!nodo.empty()){
		v.push_back(nodo.top());
		nodo.pop();
	}

	int x, y, aux;
	bool flag = false;
	vector<Task*>::iterator it;
	while(flag){
		// Sorteia 2 tarefas 
		x = rand() % v.size();
		y = rand() % v.size();
		
		while(x == y)
			y = rand() % v.size();

		if(x > y){
			aux = x;
			x = y;
			y = aux;
		}

		if(v.at(x) && v.at(y)){ //Checa se estão no caminho crítico
			if(v.at(x)->machine_id == v.at(y)->machine_id){ //Checa se são executadas na mesma máquina				
				if(checkMachine(v, x, y, v.at(x)->machine_id)){ //Checa se todas as operaçoes entre x e y são executadas na mesma máquina;
					//Case 1
					it = find(t.begin(), t.end(), v.at(y));
					it++;
					if(find(v.begin(), v.end(), (*it)) != v.end()){
						// if(){
							
						// 		Aqui é que tá o rolé!
							
						// }
					}
					// //Case 2
					it = find(t.begin(), t.end(), v.at(x));
					it--;
					if(find(v.begin(), v.end(), (*it)) != v.end()){
						it = find(t.begin(), t.end(), v.at(y));
						it--;
						if((R.dists[v.at(x)->id_task] + v.at(x)->duration) >= (R.dists[(*it)->id_task] + (*it)->duration)){
							
						}
					}


				}
			}
		}




		/*
			Checa se as Task's estão no vetor;
			Checa se são da mesma máquina;
			Checa se todas as Task's entre x e y são da mesma máquina;

		
			Caso, as sentenças acima sejam verdadeiras, aí verificamos as sentenças abaixo;
				
				case 1:
					Operação j+1 está no critical path;
					maxpath(j, last) >= maxpath(i+1, last);

				case 2: 
					Operaçao i-1 está no critical path;
					maxPath(begin, i) + i->duration >= maxPath(begin, j-1) + (j-1)->duration;
		*/

	}


}




/*
	Mostra na tela.
*/
void SimulatedAnnealing::mostraSolucao(){
	// for(int i=0; i<this->tamanho; i++)
	// 	cout << this->melhor[i] << "-";
	//cout << this->calculaCusto(this->melhor);
}

/*
Destrutor
*/
SimulatedAnnealing::~SimulatedAnnealing(){
	delete []this->melhor;
	for(int i=0; i<this->tamanho; i++)
			delete []this->custo;

}