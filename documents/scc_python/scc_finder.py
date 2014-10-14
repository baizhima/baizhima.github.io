# CS 161 Project
# Tarjan's algorithm implementation
# Shan Lu, SUNID 005997832

import sys
sys.setrecursionlimit(100000)




def create_graph(input_file):
  f = open(input_file)
  n = int(f.readline().strip())
  m = int(f.readline().strip())
  V = set()
  E = dict()
  
  for i in range(m):
    line = f.readline().strip()
    #print "line = %s" %(line)
    u = int(line.split()[0])
    v = int(line.split()[1])
    V.add(u)
    V.add(v)
    if u == v:    # self loops (u, u)
      continue
    if u not in E:
      E[u] = {v}
    else:
      E[u].add(v)     # set will avoid multiple parallel edges (u,v) and (u,v)
    
  
  return (V, E)  # return graph G 

count = 0

def tarjan(graph,largest_sccs):
  V, E = graph
  lowerlink = {}
  index = {}
  scc_sizes = []
  S = [] # init stack to be empty

  def strongconnect(v):
    global count
    index[v] = count
    lowerlink[v] = count
    count += 1
    S.append(v)

    if v in E.keys():
      for w in E[v]:
        if w not in index.keys():
          strongconnect(w)
          lowerlink[v] = min(lowerlink[v],lowerlink[w])
        elif w in S:
          lowerlink[v] = min(lowerlink[v],index[w])

    if lowerlink[v] == index[v]:
      SCC = set()
      while True:
        w = S.pop()
        SCC.add(w)
        if w == v: break
      scc_sizes.append(len(SCC))
      #print "SCC: %s" %SCC

  
  for vertex in V:
    if vertex not in index.keys():
      strongconnect(vertex)


  scc_sizes.sort(reverse=True)
  for i in range(5):
    if i < len(scc_sizes):
      largest_sccs[i] = scc_sizes[i]







'''
  Given an input file (input_file) and an array (largest_sccs) of size 5,
  fills the 5 largest scc sizes into (largest_sccs) in decreasing order.
  in decreasing order. In the case when there are less than
  5 components, you should fill 0 for the remaining values in (largest_sccs)
  (i.e if there are only 2 components of size 100 and 50, you should fill:
  largest_sccs[0] = 100
  largest_sccs[1] = 50
  largest_sccs[2] = 0
  largest_sccs[3] = 0
  largest_sccs[4] = 0
'''
def find_sccs(input_file, largest_sccs):
  G = create_graph(input_file)
  tarjan(G,largest_sccs)

''' print the output in the format we want. don't change this '''
def print_sccs(output_file, scc_sizes):
  with open(output_file, 'wb') as f:
    for i, scc_size in enumerate(scc_sizes):
      f.write('%s' % scc_size)
      if i < 4:
        f.write('\t')

'''
  Main takes two arguments: 1) input file and 2) output file.
  You should fill the find_sccs function.
'''
def main(input_file, output_file):
  largest_sccs = [0] * 5
  find_sccs(input_file, largest_sccs)
  print_sccs(output_file, largest_sccs)

if __name__ == "__main__":
  assert len(sys.argv) == 3
  main(sys.argv[1], sys.argv[2])