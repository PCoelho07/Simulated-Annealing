typedef struct Task{
	int id_task;
	int job_id;
	int machine_id;
	int duration;
	Task *pai;
	bool visited = false;

	Task(int i, int j, int m, int d) : id_task(i), job_id(j), machine_id(m), duration(d){
		// pai = new Task(0, 0, 0, 0);
	}
	
	bool operator<(const Task &t) const {
		return (duration < t.duration);
	}
	bool operator>(const Task &t) const {
		return (duration > t.duration);
	}


}Task;
