#include "scc_helper.h"



void get_degree_distribution(std::set<int> &V, std::map<int, std::set<int> > &E, char* output_file) {

	std::vector<int> vertex_count; // vertex_count[5] = 8 means 8 vertices have exactly 5 outgoing edges
	for (unsigned int i = 0; i < V.size(); i++)
		vertex_count.push_back(0);
	for (std::set<int>::iterator it = V.begin(); it != V.end(); ++it) {
		if (E.find(*it) != E.end())
			vertex_count[E[*it].size()] += 1;
		else
			vertex_count[0] += 1;
	}
	// remove redundant zeros
	while (vertex_count[vertex_count.size() - 1] == 0)
		vertex_count.pop_back();

  std::ofstream os;

  os.open(output_file);

  os << vertex_count.size() << "\n"; // n buckets
  for (unsigned int i = 0; i < vertex_count.size(); i++)
		os << vertex_count[i] << "\n";
  os.close();

}



void read_graph_incoming_edge(char* input_file, std::set<int> &vertices, std::map<int, std::set<int> > &incoming_edges) {
  std::ifstream ifs(input_file, std::ifstream::in);
  int n, m;
  ifs >> n;
  ifs >> m;
  //std::cout << "n = " << n << ", m = " << m << std::endl;
  for (int i = 0; i < m; i++) {
    int u, v;
    ifs >> u >> v;
    if (u == v) continue; // self loop (u,u)
    vertices.insert(u);
    vertices.insert(v);


    std::map<int, std::set<int> >::iterator it = incoming_edges.find(v);
    if (it != incoming_edges.end())
      (it->second).insert(u);
    else {
      std::set<int> newSet;
      newSet.insert(u);
      std::pair<int, std::set<int> > newEdge (v,newSet);
      incoming_edges.insert(newEdge);
    }
  }
}



int main(int argc, char* argv[]) {
	assert(argc == 4);
	std::set<int> V;
	std::map<int, std::set<int> > E;


	printf("\n\n\nRead incoming edge info...\n");
	read_graph_incoming_edge(argv[1],V,E);
	printf("Extracting incoming degree distribution information..\n");
	get_degree_distribution(V,E,argv[2]);
	printf("Finish writing incoming degree info into %s\n\n",argv[2]);

	printf("Read outgoing edge info...\n");
	read_graph(argv[1], V, E);
	printf("Extracting outgoing degree distribution information..\n");
	get_degree_distribution(V, E, argv[3]);
	printf("Finish writing outgoing degree info into %s\n",argv[3]);


	return 0;
}