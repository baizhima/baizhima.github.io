#include "scc_helper.h"



void DFS_iterative(std::set<int> &V, std::map<int, std::set<int> > &E) {

	int n = V.size();
	int maximum_diameter = 0;
	int max_diam_source = 0, max_diam_dest = 0;
	for (int i = 0; i < n; i++) {
		std::stack<int> S;
		std::set<int> visited;
		std::map<int, int> depth;
		int current_depth = 0;
		int source = i + 1;
		printf("source: %d\n",source);
		int last_vertex = 0;
		S.push(source);
		while (!S.empty()) {
			int vertex = S.top();
			S.pop();
			printf("starting from %d\n",vertex);
			if (visited.find(vertex) == visited.end()) {
				current_depth++;
				visited.insert(vertex);
				std::set<int> edgeSet = E[vertex];
				for (std::set<int>::iterator it = edgeSet.begin(); it != edgeSet.end(); ++it) {
					S.push(*it);
					depth[*it] = current_depth;
					last_vertex = *it;
					printf("find %d\n",*it);
				}
				if (current_depth > maximum_diameter) {
					maximum_diameter = current_depth;
					max_diam_source = source;
					max_diam_dest = last_vertex;
					printf("UPDATE maximum_diameter %d: from %d to %d\n", maximum_diameter,max_diam_source, max_diam_source);

				}
			}

		}

	}
	printf("maximum_diameter = %d, from %d to %d\n",maximum_diameter, max_diam_source, max_diam_dest);
}


void BFS_iterative(std::set<int> &V, std::map<int, std::set<int> > &E, char * input_file) {

	int n = V.size();
	int maximum_diameter = 0;
	long long int path_length_cumulative = 0;
	long long int path_count = 0;
	int max_diam_source = 0, max_diam_dest = 0;
	for (int i = 0; i < n; i++) {
		std::queue<int> Q;
		std::set<int> visited;
		std::map<int, int> depth;

		int source = i + 1;
		depth[source] = 0;
		//printf("\n\nsource: %d\n",source);
		int last_vertex = 0;
		Q.push(source);
		while (!Q.empty()) {
			int vertex = Q.front();
			Q.pop();
			path_length_cumulative += depth[vertex];
			path_count++;
			//printf("starting from %d\n",vertex);
			if (visited.find(vertex) == visited.end()) {
				visited.insert(vertex);
				for (std::set<int>::iterator it = E[vertex].begin(); it != E[vertex].end(); ++it) {
					if (visited.find(*it) != visited.end()) continue;
					Q.push(*it);
					depth[*it] = depth[vertex] + 1;
					last_vertex = *it;
					//printf("find %d, depth = %d\n",*it, depth[*it]);
				}
				if (depth[vertex] > maximum_diameter) {
					maximum_diameter = depth[vertex];
					max_diam_source = source;
					max_diam_dest = last_vertex;
					//printf("UPDATE maximum_diameter %d: from %d to %d\n", maximum_diameter,max_diam_source, max_diam_dest);

				}
			}

		}
		if (i % 100 == 0) {
			printf("The program has checked %d sources, about %f percent of total work\n", i, i * 100.0/n);
			double current_avg_path_len = path_length_cumulative * 1.0 / path_count;
			printf("current avg path length = %f, maximum_diameter = %d", current_avg_path_len, maximum_diameter);
			printf(", from %d to %d\n\n", max_diam_source, max_diam_dest);
		}
	}
	printf("\n\n%s network information:\n",input_file);
	printf("maximum_diameter = %d, from %d to %d\n",maximum_diameter, max_diam_source, max_diam_dest);
	printf("avg shortest path length = %f\n, path_count = %lld\n", path_length_cumulative * 1.0 / path_count, path_count);
}

// argv[1]: graph.txt

int main(int argc, char* argv[]) {
	assert(argc == 2);
	std::set<int> V;
	std::map<int, std::set<int> > E;
	read_graph(argv[1], V, E);
	BFS_iterative(V,E, argv[1]);
	return 0;
}