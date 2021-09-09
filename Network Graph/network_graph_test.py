import networkx as nx
import matplotlib.pyplot as plt

G = nx.Graph()

for i in range(100):
    G.add_edge(0, i)

pos = nx.circular_layout(G)
nx.draw(G, pos, nlist=[range(5, 10), range(5)], with_labels=True)
plt.subplot(121)
nx.draw_shell(G, with_labels=True, font_weight='bold')

plt.show()