import PairtreeOrchard as m


print("decision tree test 1 - is built")
dt = m.DecisionTree()
assert dt.is_built == False
del dt

print("decision tree test 2 - add node")
nd = m.Node()
dt = m.DecisionTree()
dt.AddNodeToDT(nd)
assert len(dt.nodes) == 1
del dt
del nd

print("decision tree test 3 - print")
nd = m.Node()
dt = m.DecisionTree()
dt.AddNodeToDT(nd)
dt.Print()
del dt
del nd

print("done.")

