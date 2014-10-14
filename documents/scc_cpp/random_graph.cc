#include <assert.h>
#include <cstdlib>
#include <chrono>
#include <random>
#include <fstream>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstring>
//#include "scc_finder.h"


void read_graph(char* input_file, std::set<int> &vertices, std::map<int, std::set<int> > &edges) {
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
    std::map<int, std::set<int> >::iterator it = edges.find(u);
    if (it != edges.end())
      (it->second).insert(v);
    else {
      std::set<int> newSet;
      newSet.insert(v);
      std::pair<int, std::set<int> > newEdge (u,newSet);
      edges.insert(newEdge);
    }
  }

  ifs.close();
}

bool scc_sort(std::set<int> s1, std::set<int> s2) {
  return s1.size() > s2.size();
}

void find_sccs(char *input_file, std::array<unsigned, 5> &largest_sccs){
  // Graph G(V,E) is stored in one set V and one map E(v->edges[v])
  std::set<int> vertices;
  std::map<int, std::set<int> > edges;

  std::set<int> identified;
  std::map<int, int> index;
  std::stack<int> stack;
  std::stack<int> boundaries;

  std::vector<std::set<int> > SCCs;

  read_graph(input_file, vertices, edges);

  for (std::set<int>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
    int u = *it;

    if (index.find(u) == index.end()) {
      //printf("index does not contain %d\n",u);
      std::stack<std::pair<int, int> > visit_stack;
      // 1:'visit u', 2:'visit u's edges', 3:'finish visiting u'
      //std::pair<int, int> tmp (1, u);
      visit_stack.push(std::pair<int, int> (1, u));
      while (!visit_stack.empty()) {
        std::pair<int, int> current = visit_stack.top();
        visit_stack.pop();
        int type = current.first;
        int v = current.second;
        //printf("type = %d, v = %d\n", type, v);
        if (type == 1) {
          index[v] = stack.size();
          stack.push(v);
          boundaries.push(index[v]);
          visit_stack.push(std::pair<int, int> (3, v));
          std::set<int> edgeSet = edges[v];
          for (std::set<int>::iterator it2 = edgeSet.begin(); it2 != edgeSet.end(); ++it2) {
            visit_stack.push(std::pair<int, int> (2, *it2));
          }
        }
        else if (type == 2) {
          if (index.find(v) == index.end())
            visit_stack.push(std::pair<int, int> (1, v));
          else if (identified.find(v) == identified.end())
            while (index[v] < boundaries.top())
              boundaries.pop();
        } // type == 2
        else {
          if (boundaries.top() == index[v]) {
            boundaries.pop();
            std::set<int> newScc;
            int start = index[v];
            int end = stack.size() - 1;
            for (int i = end; i >= start; i--){
              newScc.insert(stack.top());
              stack.pop();
            }
            identified.insert(newScc.begin(),newScc.end());
            SCCs.push_back(newScc);
          }
        } // type == 3
      } // while (visit_stack not empty)
    } // if u is in index
  }
  // sort SCCs by their size in decreasing order
  std::sort(SCCs.begin(),SCCs.end(),scc_sort);
  for (unsigned int i = 0; i < 5; i++) {
    if (SCCs.size() > i)
      largest_sccs[i] = SCCs[i].size();
    else
      largest_sccs[i] = 0;
  }

}


void write_graph(int n, int m, char *output_file){
  std::ofstream os;
  os.open(output_file);
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

  std::minstd_rand0 g1 (seed);
  os << n << "\n";
  os << m << "\n";

  for (int i = 0; i < m; i++){
    int u = g1() % n + 1;  // u from 1 to n
    int v = g1() % n + 1;  // v from 1 to n
    while (u == v)
    	v = g1() % n + 1; // avoid (u,u) self loop

    os << u << " " << v << "\n";
  }
  os.close();
}

void test_giant_component(int n, int m, int trials) {
	int count = 0;
	for (int i = 0; i < trials; i++) {
		char filename[50] = "trial_random.txt";
		write_graph(n, m, filename);
		std::array<unsigned, 5> scc_sizes;
		find_sccs(filename, scc_sizes);
		if (scc_sizes[1] == 0)
			count += 1;
	}
	std::cout << "ratio:" << count * 1.000 / trials << std::endl;
}


void get_real_network_info() {
    char names[10][30] = {"email_enron.txt","email_eu_all.txt","soc_epinions.txt","soc_slashdot_0811.txt","web_google.txt",
                          "random_email_enron.txt", "random_email_eu_all.txt","random_soc_epinions.txt",
                          "random_soc_slashdot_0811.txt","random_web_google.txt"};
    int graph_n[5] = {36694,265216,75880,77362,875714};
    int graph_m[5] = {367664,418960,508839,828164,5105042};
    char graphname[50];
    int num;
      std::cout << "find the one you need to know:" << std::endl;
      for (int i = 0; i < 10; i++)
        std::cout << i << ": " << names[i] << std::endl;
      std::cin >> num;
      assert(num >= 0 && num < 10);
      std::cout << "get network " << names[num]<< " data" << std::endl;
    strcpy(graphname,names[num]);


    if (num >= 5) {
      std::cout << "generating random graph with n = " << graph_n[num-5] << ", m = " << graph_m[num-5] << std::endl;
      int scc_total[5] ={0,0,0,0,0};
      for (int i = 0; i < 5; i++) {
        std::array<unsigned, 5> scc_sizes;
        std::cout << "Round #" << i << std::endl;
        write_graph(graph_n[num-5],graph_m[num-5],graphname);
        std::cout << "find SCCs..." << std::endl;
        find_sccs(graphname,scc_sizes);
        for (int j = 0; j < 5; j++) {
          std::cout << scc_sizes[j] << " " ;
          scc_total[j] += scc_sizes[j];
        }
        std::cout << "\n";
      }
      std::cout << "average top 5 SCC sizes: ";
      for (int j = 0; j < 5; j++) {
          std::cout << scc_total[j] / 5.000 << " " ;
      }
      std::cout << "\n";
    }
    else {
      std::array<unsigned, 5> scc_sizes;
      find_sccs(graphname,scc_sizes);
      for (int j = 0; j < 5; j++)
          std::cout << scc_sizes[j] << " " ;
        std::cout << "\n";
  }


}

/*
 * Main takes three arguments:
 * 1). number of nodes
 * 2). Number of edges
 * 3). output file
 */
int main(int argc, char **argv){
  get_real_network_info();
/*
  assert(argc == 4);
  unsigned n = strtoul(argv[1], NULL, 0);
  unsigned m = strtoul(argv[2], NULL, 0);
  char *output_file = argv[3];
  write_graph(n,m,output_file);
  //int trials = 3000;
  //test_giant_component(n, m,trials);
*/
  return 0;
}