#include "sa.hpp"

SimulatedAnnealing::SimulatedAnnealing(const char* strT0, double t0, const char* strNmax, int nmax,
									const char* txredux, float talpha, Dgraph *d) { 

	double tf = 0; // Temperatura final
	double t = t0; // Temṕeratura corrente
	double txReduc = talpha; // Taxa de redução da temperatura
	
	Dgraph *d_graph = d;
	makespan corrente, vizinho;

	// this->solucaoInicial(d_graph);
	this->melhor = corrente = this->calculaCusto(d, NULL);
	
	// /* Variáveis auxilares */
	int count = 0;
	double a = 0, p = 0;
	int diff = 0;

	// /* --  LOOP Principal  -- */
	cout << "Entrou no loop principal \n";
	while(t > 0.0001) {
		++count;
		
		for(int contador = 0; contador < nmax; contador++) {
			// cout << t << "\n";
			this->solucaoVizinha(d_graph, corrente);
			vizinho = this->calculaCusto(d_graph, NULL);
			diff = vizinho.custo - corrente.custo;

			a =double(double(rand())/double(RAND_MAX));
			p =exp(double(-diff/t));
			
			if(diff < 0 || a < p) {
				corrente = vizinho;
				if (corrente.custo < melhor.custo) {
					this->melhor = corrente;
					cout << this->melhor.custo << "\n";
				}
			}
		
		}		

		t = txReduc * t; // Reduz a temperatura
		if (count % 200 == 0) {
			nmax += 10;
		}


	}		

	cout << this->melhor.custo << "\n";
}


void visits(Task *t, list<Task*> &l, vector<Task*> *adj) {

	t->visited = true;

	vector<Task*>::iterator i;

	if(adj[t->id_task].size() > 0) {
		// cout << t->id_task << "\n";
		// cout << "\nPossui adjacentes!\n";
		for(i = adj[t->id_task].begin(); i != adj[t->id_task].end(); ++i) {
			// cout << "loop for \n";
			// cout << (*i)->visited << "\n";
			if((*i)->visited == false){
				visits((*i), l, adj);
			}

			// cout << aux << "\n";
			// aux++;
			
		}
	}

	l.push_front(t);
}



/*
	Retorna uma ordenação topológica
*/
list<Task*> ordTop(Dgraph *d_graph, Task *v){
	vector<Task*> vertex = d_graph->getTaskList();
	vector<Task*> *graph = d_graph->getGraph();
	vector<Task*> s; // Lista de vertices sem precedência.
	list<Task*> l; // Lista ordenada topologicamente.	
	
	for(int i = 0; i < vertex.size(); i++) {
		if(vertex.at(i)->visited  == false)	
			visits(vertex.at(i), l, graph);
	}

	return l;
}

void show_ord(list<Task*> l) {
	list<Task*>::iterator it = l.begin(); 
	while(l.size() > 0) {
		cout << l.front()->id_task << "\n";
		l.pop_front();
	}
}


void stack_up(stack<Task*> &q, Task *f) {
	if (f != NULL) {
		 q.push(f);
		 stack_up(q, f->pai);
	}
}


/*
	Dado um grafo disjuntivo, calcula o makespan
*/
 makespan SimulatedAnnealing::calculaCusto(Dgraph *d_graph, list<Task*> *ordem) {
	list<Task*> ord;
	cout << "\n Passou! \n ";	
	if(ordem == NULL){
		ord = ordTop(d_graph, NULL);
		cout << " -- Ordem topológica --\n";
		show_ord(ord);
	}
	else
		ord = *(ordem);

	vector<Task*> *graph = d_graph->getGraph();
	int *dists = new int[d_graph->getTaskList().size()];
	stack<Task*> nodes_cp; // Pilha de elementos do caminho crítico

	dists[d_graph->getTaskList().at(0)->id_task] = 0;
	for(int i=1; i < d_graph->getTaskList().size(); i++){
		dists[d_graph->getTaskList().at(i)->id_task] = INFNEG;
	}

	list<Task*>::iterator v; 
	int aux = 0;
	for(v = ord.begin(); v != ord.end(); v++){
		for(int i= 0; i < graph[(*v)->id_task].size(); ++i){

			if(dists[graph[(*v)->id_task].at(i)->id_task] < (dists[(*v)->id_task] + graph[(*v)->id_task].at(i)->duration)){
				aux++;
				dists[graph[(*v)->id_task].at(i)->id_task] = dists[(*v)->id_task] + graph[(*v)->id_task].at(i)->duration;
				// graph[(*v)->id_task].at(i)->pai = new Task((*v)->id_task, (*v)->job_id, (*v)->machine_id, (*v)->duration); 
				graph[(*v)->id_task].at(i)->pai = (*v);

				// cout << " Iterations: " << aux << " \n";
				// cout << "Node: " << graph[(*v)->id_task].at(i)->id_task << " parents: " << graph[(*v)->id_task].at(i)->pai->id_task << "\n";
			}

		}
	}
	cout << "-- Distancias -- \n";
	// vector<Task*>::iterator it = d_graph->getTaskList().begin();
	for(int i = 0; i < d_graph->getTaskList().size(); i++) {
		// cout << (*it)->id_task << ":" << dists[(*it)->id_task] << "\n"; 
		cout << d_graph->getTaskList().at(i)->id_task << ":" << dists[d_graph->getTaskList().at(i)->id_task] << "\n";
	}



	// Exibição
	cout << " -- Exibição -- \n";
	for(int i=0; i < d_graph->getTaskList().size(); ++i){
		cout << "Node: " << d_graph->getTaskList().at(i)->id_task << "\n";
		for(int j = 0; j < graph[i].size(); ++j){
			cout << " parents: " << graph[i].at(j)->pai->id_task << "\n";
		}
	}

	cout << "entrou \n";

	// vector<Task*>::iterator t = d_graph->getTaskList().end();
	// --t;
	// cout << (*t)->id_task << "\n";
	// cout << (*t)->pai->id_task << "\n";
	// stack_up(nodes_cp, (*t));	
	cout << "CP size: " << nodes_cp.size() <<  "\n";




	exit(-1);
	makespan R;
	R.custo = dists[101]; //Custo do critical path
	R.nodos = nodes_cp; // Nós que compõe o critical path
	R.dists = dists;
	// makespan R

	return R; //Estrutura que contém o custo e os nodos do caminho crítico;
	
	/*
		Pela propriedade de relaxamento de caminho(Lema 24.15 - Livro Cormen), se existir um caminho do 's'(source) e 't'(target),
		dists[t] = minpath(s, t), nesse caso, dists[t] = maxpath(s, t).
	*/
}

bool sortByDuration(const Task *t1, const Task *t2) {
	if(t1->duration < t2->duration ) {
		return true;
	}
	return false;
}

/*
	Regra shortest path time
*/
void SimulatedAnnealing::solucaoInicial(Dgraph *d_graph){
	cout << "Solução inicial \n";
	int qtdMach = d_graph->getMach();
	vector<Task*> *graph = d_graph->getGraph();
	this->m = new vector<Task*>[qtdMach];
	vector<Task*> *m0 = new vector<Task*>[qtdMach]; // Vazamento!!!
	
	for(int i =0; i < qtdMach; i++){	
		for(int j = 0; j < d_graph->getTaskList().size(); j++){
			if(d_graph->getTaskList().at(j)->machine_id == i)
				this->m[i].push_back(d_graph->getTaskList().at(j));
		}
	}

	//Para cada máquina, executa SPT
	for(int i = 0; i < qtdMach; i++){
		sort(this->m[i].begin(), this->m[i].end(), sortByDuration); // O(N*log n)
	}



	// //Adiciona as arestas no grafo
	for(int i =0; i < qtdMach; i++){
		for(int j=0; j < this->m[i].size()-1; j++){
			d_graph->addEdge(this->m[i][j], this->m[i][j+1]);
			d_graph->addEdge(this->m[i][j+1], this->m[i][j]);
		}
	}

	delete [] m0;
	delete [] m;
}

/*
	
Verifica se todas as operações entre i e j no caminho crítico, são da mesma máquina (Def de par crítico)
	Supondo que i e j são da mesma máquina

*/

bool checkSucessiveMachine(vector<Task*> v, int i, int j, int idMachine){
	vector<Task*>::iterator x = find(v.begin(), v.end(), v.at(i));
	vector<Task*>::iterator y = find(v.begin(), v.end(), v.at(j));
	vector<Task*>::iterator it = x;

	if( (*x)->machine_id == (*y)->machine_id ) {
		if(y == (it++)) {
			return true;
		}		
	}

	return false;
}

/*
	Inverte a ordem que i e j são processadas na mesma máquina
*/
void swapTask(Dgraph *dg, vector<Task*> *graph, int i, int j) {
	// Adiciono as novas arestas revertendo a ordem
	Task* pai_i = dg->getTaskList().at(i)->pai;
	
	dg->addEdge(pai_i, dg->getTaskList().at(j));
	dg->addEdge(dg->getTaskList().at(i), graph[j].at(0));
	dg->addEdge(dg->getTaskList().at(j), dg->getTaskList().at(i));

	dg->delEdge(pai_i, dg->getTaskList().at(i));
	dg->delEdge(dg->getTaskList().at(i), dg->getTaskList().at(j));
	dg->delEdge(dg->getTaskList().at(j), graph[j].at(0));
}

/*
	Gera uma solução vizinha à que foi passada na função.

	**** EM CONSTRUÇÃO ****
*/
void SimulatedAnnealing::solucaoVizinha(Dgraph *d_graph, makespan R){
	vector<Task*> t = d_graph->getTaskList();
	int custo = R.custo;
	stack<Task*> nodo = R.nodos;
	vector<Task*> v; // Critical path
	int id_mach;
	int qtdMach = d_graph->getMach();
	vector<Task*> *graph = d_graph->getGraph();

	vector<Task*> *m0 = new vector<Task*>[qtdMach]; // lista de maquinas e suas respectivas operações

	while(!nodo.empty()){
		v.push_back(nodo.top());
		nodo.pop();
	}

	int x = 0, y = 0, aux;
	bool flag = false;
	vector<Task*>::iterator it;
	
	srand(time(NULL));
	while(flag == false) {
		// Sorteia 2 tarefas 
		x = rand() % v.size();
		y = rand() % v.size();
		
		cout << "Tamanho caminho critico: " << v.size() << "\n";

		while(x == y){
			y = rand() % v.size();
		}

		cout << " -- Números sorteados -- \n";
		cout << x << "\n"; 
		cout << y << "\n";

		if(x > y){
			aux = x;
			x = y;
			y = aux;
		}

		if(v.at(x) && v.at(y)){ //Checa se estão no caminho crítico
			if(checkSucessiveMachine(v, x, y, v.at(x)->machine_id)){ //Checa se todas as operaçoes entre x e y são executadas na mesma máquina;
				swapTask(d_graph, graph, x, y);
				flag = true;
			}	
		}
	}
		/*
			Pseudocódigo:

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
	delete []this->m;
	
	for(int i=0; i<this->tamanho; i++)
			delete []this->custo;

}