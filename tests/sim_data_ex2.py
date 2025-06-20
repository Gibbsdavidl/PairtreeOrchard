# tree sim data 1
#
#         node1: A
#        /         \
#    node2: B     node3 → class1
#   /     \
#node4  node5
# ↓       ↓
#class2 class1


import pandas as pd
import numpy as np

def generate_tree_dataset(N, random_state=42):
    np.random.seed(random_state)
    
    # Allocate roughly:
    # - 40% go to node3 (A=1)
    # - 60% go to node2 (A=0)
    #     - within that, B=0 → class2, B=1 → class1
    n_node3 = int(0.4 * N)
    n_node2 = N - n_node3

    # Split node2 into class2 and class1 (e.g., 50/50)
    n_class2 = n_node2 // 2
    n_class1_node2 = n_node2 - n_class2

    # Now generate rows
    rows = []

    # node3 → A=1 → class1
    for _ in range(n_node3):
        rows.append({'A': 1, 'B': np.random.randint(0, 2), 'label': 'class1'})  # B irrelevant

    # node4 → A=0, B=0 → class2
    for _ in range(n_class2):
        rows.append({'A': 0, 'B': 0, 'label': 'class2'})

    # node5 → A=0, B=1 → class1
    for _ in range(n_class1_node2):
        rows.append({'A': 0, 'B': 1, 'label': 'class1'})

    # Shuffle the dataset
    df = pd.DataFrame(rows)
    df = df.sample(frac=1, random_state=random_state).reset_index(drop=True)
    return df

df = generate_tree_dataset(100)

df.to_csv('sim_data_ex2.csv', index=None)
