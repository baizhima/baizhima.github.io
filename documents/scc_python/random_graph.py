import sys
import random
import scc_finder


def generate_graph(n, m):
    V = set([i+1 for i in range(n)])
    E = dict()
    for i in range(m):
        u, v = 0, 0
        while u == v:   
            u = random.randint(1,n)
            v = random.randint(1,n)
        if u not in E:
            E[u] = {v}
        else:
            E[u].add(v)
    return (V, E)





def tarjan_random(graph):
  count = 0
  V, E = graph
  lowerlink = {}
  index = {}
  scc_sizes = []
  S = [] # init stack to be empty

  def strongconnect(v,count):
    
    index[v] = count
    lowerlink[v] = count
    count += 1
    S.append(v)

    if v in E.keys():
      for w in E[v]:
        if w not in index.keys():
          strongconnect(w,count)
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
      strongconnect(vertex,count)
  return len(scc_sizes) == 1


'''
  main takes three arguments:
  1). number of nodes
  2). Number of edges
  3). output file
'''
def main(n, m, output_file):
  trials = 1000
  scc_count = 0
  for i in range(trials):
    G = generate_graph(n,m)
    if tarjan_random(G):
        scc_count += 1
  print "strong connected ratio:%f" %(scc_count/float(trials))

if __name__ == "__main__":
  assert len(sys.argv) == 4
  main(int(sys.argv[1]), int(sys.argv[2]), sys.argv[3])


