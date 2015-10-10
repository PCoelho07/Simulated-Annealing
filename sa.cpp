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
	Dado um grafo disjuntivo, calcula seu makespan
*/
float SimulatedAnnealing::calculaCusto(Dgraph*){
	float custoFinal = 0;
	
	
	return custoFinal;
}

/*
	Regra SPT
*/
void SimulatedAnnealing::solucaoInicial(Dgraph *d_graph){
	int qtdMach = d_graph->getMach();
	vector<Task*> *graph = d_graph->getGraph();
	vector<Task> *m = new vector<Task>[qtdMach];
	vector<Task*> *m0 = new vector<Task*>[qtdMach];
	
	for(int i =0; i < qtdMach; i++){	
		for(int j = 0; j < d_graph->getTaskList().size(); j++){
			if(d_graph->getTaskList().at(j)->machine_id == i)
				m[i].push_back((*d_graph->getTaskList().at(j)));
		}
	}

	//Para cada máquina, executa SPT
	for(int i = 0; i < qtdMach; i++){
		sort(m[i].begin(), m[i].end()); // O(N*log n)
	}

	//Adiciona as arestas no grafo
	for(int i =0; i < qtdMach; i++){
		for(int j=0; j < m[i].size()-1; j++){
			d_graph->addEdge(&m[i][j], &m[i][j+1]);
		}
	}
}
/*
	Gera uma solução vizinha à que foi passada na função.
*/
void SimulatedAnnealing::solucaoVizinha(int *corrente, int* vizinho){
	// int *vizinho = new int[this->tamanho];
	memcpy(vizinho, corrente, this->tamanho*sizeof(int));

	// srand(time(NULL));
	int x = rand() % this->tamanho;
	int y = rand() % this->tamanho;
	int aux = 0;

	while(x == y)
		y = rand() % this->tamanho;

	aux = vizinho[x];
	vizinho[x] = vizinho[y];
	vizinho[y] = aux;
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