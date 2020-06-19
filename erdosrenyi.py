import networkx as nx
n = int(input("Enter #vertices: "))
p = float(input("Enter Edge probability: "))
er = nx.erdos_renyi_graph(n, p)
f_name = raw_input("Enter file name: ")
f = open(f_name,"w")
for x in er.edges():
   f.write(str(x[0])+ ' ' + str(x[1]))
   f.write('\n')
print str(er.number_of_nodes()) + ' ' + str(er.number_of_edges())
