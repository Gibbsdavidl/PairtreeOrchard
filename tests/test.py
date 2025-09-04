import PairtreeOrchard as po
import numpy as np
import pandas as pd
from sklearn.preprocessing import LabelEncoder

print("\n\ndecision tree test 1 - is built")
dt = po.DecisionTree()
assert dt.is_built == False
del dt

print("\n\ndecision tree test 2 - add node")
nd = po.Node()
dt = po.DecisionTree()
dt.add_node(nd)
assert len(dt.nodes) == 1
del dt
del nd

print("\n\ndecision tree test 3 - print")
nd = po.Node()
dt = po.DecisionTree()
dt.add_node(nd)
dt.print_tree()
del dt
del nd


print("\n\ndecision tree test 4 - build tree 1")
X = np.random.rand(5, 3)  # features are in rows, samples in columns
Y =  np.random.randint(0, 1, size=5)
cl = po.DecisionTreeClassifier()
cl.build_tree(X,Y)
cl.print()
del cl


print("\n\ndecision tree test 5 - sim data ex 2")
df = pd.read_csv('tests/sim_data_ex2.csv')
# Features
X = df[['A', 'B']].to_numpy()
# Labels as integers (e.g., class1 → 0, class2 → 1)
le = LabelEncoder()
Y = le.fit_transform(df['label'])
# build tree
cl = po.DecisionTreeClassifier()
cl.build_tree(X,Y)
cl.print()


print("done.")


