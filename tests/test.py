import PairtreeOrchard as po
import numpy as np

print("decision tree test 1 - is built")
dt = po.DecisionTree()
assert dt.is_built == False
del dt

print("decision tree test 2 - add node")
nd = po.Node()
dt = po.DecisionTree()
dt.add_node(nd)
assert len(dt.nodes) == 1
del dt
del nd

print("decision tree test 3 - print")
nd = po.Node()
dt = po.DecisionTree()
dt.add_node(nd)
dt.print_tree()
del dt
del nd


print("decision tree test 4 - build tree 1")
X = np.random.rand(5, 3)
Y =  np.random.randint(0, 1, size=5)
cl = po.DecisionTreeClassifier()
cl.build_tree(X,Y)
cl.print()
del cl


print("done.")

