rg: random_graph.cc scc_helper.cc
	g++ -std=c++11 -march=native -Wall -O4 random_graph.cc scc_helper.cc -o randomgraph

nd: network_diameter.cc scc_helper.cc
	g++ -std=c++11 -march=native -Wall -O4 network_diameter.cc scc_helper.cc -o network_diameter

dd: degree_distribution.cc scc_helper.cc
	g++ -std=c++11 -march=native -Wall -O4 degree_distribution.cc scc_helper.cc -o degree_distribution


clean:
	rm -f randomgraph network_diameter degree_distribution