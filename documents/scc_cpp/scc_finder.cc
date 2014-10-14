// CS 161 Project
// Author: Shan Lu


#include <assert.h>
#include <array>
#include "scc_finder.h"

#include <cstdio>



/* Given an input file name, this function will store data into G(V,E)
 *
 */

void read_graph(char* input_file, std::set<int> &vertices, std::map<int, std::set<int> > &edges) {
  std::ifstream ifs(input_file, std::ifstream::in);
  int n, m;
  int returned_v;
  freopen(input_file,"r",stdin);
  returned_v = scanf(" %d", &n);
  returned_v = scanf(" %d", &m);

  //std::cout << "n = " << n << ", m = " << m << std::endl;
  for (int i = 0; i < m; i++) {
    int u, v;

    returned_v = scanf(" %d %d", &u, &v);
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
  /*
  for (std::set<int>::iterator it=vertices.begin(); it!=vertices.end(); ++it) {
    std::cout << *it << ": { ";
    std::set<int> edgeSet = edges[*it];
    for (std::set<int>::iterator it2 = edgeSet.begin(); it2 != edgeSet.end(); ++it2)
      std::cout << *it2 << " ";
    std::cout << "}" << std::endl;
  }*/
  ifs.close();
}


bool scc_sort(std::set<int> s1, std::set<int> s2) {
  return s1.size() > s2.size();
}

/**
 * Given an input file (input_file) and an array (largest_sccs) of size 5,
 * fills the 5 largest scc sizes into (largest_sccs) in decreasing order.
 * in decreasing order. In the case when there are less than
 * 5 components, you should fill 0 for the remaining values in (largest_sccs)
 * (i.e if there are only 2 components of size 100 and 50, you should fill:
 * largest_sccs[0] = 100
 * largest_sccs[1] = 50
 * largest_sccs[2] = 0
 * largest_sccs[3] = 0
 * largest_sccs[4] = 0
 */
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

/* print the output in the format we want. don't change this */
void print_sccs(char *output_file, std::array<unsigned, 5> &scc_sizes){
  std::ofstream os;
  os.open(output_file);
  for (unsigned i = 0; i < scc_sizes.size(); i++){
    os << scc_sizes[i];
    if (i < 4) os << "\t";
  }
  os.close();
}

/*
 * Main takes two arguments: 1) input file and 2) output file.
 * You should fill the find_sccs function.
 */

int main(int argc, char **argv){
  assert(argc == 3);

  std::array<unsigned, 5> scc_sizes;
  find_sccs(argv[1], scc_sizes);

  // Output the first 5 sccs into a file.
  print_sccs(argv[2], scc_sizes);
  return 0;
}