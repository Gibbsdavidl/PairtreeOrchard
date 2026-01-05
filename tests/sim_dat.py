import numpy as np
import pandas as pd

class TreeNode:
    def __init__(self, node_id):
        self.node_id = node_id
        self.left = None
        self.right = None
        self.split_feature = None
        self.threshold = None
        self.label = None  # Leaf label if no children

def build_tree_from_edges(edges):
    nodes = {}
    for parent, child, feature, threshold, direction in edges:
        if parent not in nodes:
            nodes[parent] = TreeNode(parent)
        if child not in nodes:
            nodes[child] = TreeNode(child)

        child_node = nodes[child]
        parent_node = nodes[parent]
        parent_node.split_feature = feature
        parent_node.threshold = threshold

        if direction == 'L':
            parent_node.left = child_node
        elif direction == 'R':
            parent_node.right = child_node
        else:
            raise ValueError("Direction must be 'L' or 'R'")
    return nodes[0]  # assuming node 0 is root

def generate_constraints_from_path(path):
    constraints = {}
    for node, direction in path:
        feature = node.split_feature
        threshold = node.threshold
        if feature not in constraints:
            constraints[feature] = (-np.inf, np.inf)
        lo, hi = constraints[feature]
        if direction == 'L':
            constraints[feature] = (lo, min(hi, threshold))
        else:
            constraints[feature] = (max(lo, np.nextafter(threshold, np.inf)), hi)
    return constraints

def generate_samples_for_leaf(constraints, n_features, label, n_samples=5):
    X = np.random.uniform(0, 1, size=(n_samples, n_features))
    for feature, (lo, hi) in constraints.items():
        X[:, feature] = np.random.uniform(lo + 1e-3, hi - 1e-3, size=n_samples)
    y = np.full(n_samples, label)
    return X, y

def dfs_collect(node, path=[], label_counter=[0], n_features=2):
    if node.left is None and node.right is None:
        # Leaf
        constraints = generate_constraints_from_path(path)
        X, y = generate_samples_for_leaf(constraints, n_features, label_counter[0])
        label_counter[0] += 1
        return X, y

    X_total, y_total = [], []
    if node.left:
        X_left, y_left = dfs_collect(node.left, path + [(node, 'L')], label_counter, n_features)
        X_total.append(X_left)
        y_total.append(y_left)
    if node.right:
        X_right, y_right = dfs_collect(node.right, path + [(node, 'R')], label_counter, n_features)
        X_total.append(X_right)
        y_total.append(y_right)

    return np.vstack(X_total), np.concatenate(y_total)

# EXAMPLE
if __name__ == "__main__":
    # (parent_id, child_id, feature_index, threshold, direction)
    edge_list = [
        (0, 1, 0, 0.5, 'L'),
        (0, 2, 0, 0.5, 'R'),
        (1, 3, 1, 0.3, 'L'),
        (1, 4, 1, 0.3, 'R'),
        # nodes 2,3,4 are leaves
    ]

    root = build_tree_from_edges(edge_list)
    n_features = 2
    X, y = dfs_collect(root, n_features=n_features)

    df = pd.DataFrame(X, columns=[f"f{i}" for i in range(n_features)])
    df["label"] = y
    print(df)

    # Optionally, verify with sklearn
    from sklearn.tree import DecisionTreeClassifier
    clf = DecisionTreeClassifier()
    clf.fit(X, y)
    print("Depth:", clf.get_depth())
