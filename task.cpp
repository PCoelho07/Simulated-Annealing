typedef struct Task{
	int id_task;
	int job_id;
	int machine_id;
	int duration;
	Task *pai;


	Task(int i, int j, int m, int d) : id_task(i), job_id(j), machine_id(m), duration(d){
		// pai = NULL;
	}
	
	bool operator<(const Task &t) const {
		return (duration < t.duration);
	}
	bool operator>(const Task &t) const {
		return (duration > t.duration);
	}


}Task;
