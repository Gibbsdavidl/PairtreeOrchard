import PairtreeOrchard as po


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

print("decision tree test 4 - init with param")
cl = po.DecisionTreeClassifier(1,2,3,4,5.5)
cl.add_node()
cl.print()
del cl

print("done.")

