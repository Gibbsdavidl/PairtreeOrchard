#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

#include <limits>
#include <vector>
#include <random>
#include <string>

//#include "splitter.h"

enum MaxFeaturesMethod { sqrt_method, log2_method, all_method };

struct Record {         // about the data contained in the node
  int n_samples=0;      // number of samples in the set
  double entropy=0.0;   // entropy of this node
  std::vector<int> index;  // index in to data and labels
  int variable1=0;
  int variable2=0; // v1 > v2
  double prob_v1gtv2=0.0;  // of v1 > v2 / n_samples
};


struct Node {       // about the node in the tree
  int depth=0;          // depth in the tree
  int parent_id=0;
  int self_id=0;
  int left_child_id=0;
  int right_child_id=0;
  bool is_left=false;
  bool is_leaf=false;
  Record record_;
  void print_node();
};


struct NodeStack {
  std::vector<Node> stack;
  void push(Node record);
  void pop(Node &record);
  bool is_empty();
};


struct RecordStack {
  std::vector<Record> stack;
  void push(Record record);
  void pop(Record &record);
  bool is_empty();
};


struct DecisionTree {
  std::vector<Node> nodes;  // tree stored as vector of nodes
  bool is_built = false;    // is the tree constructed yet?

  int add_node(Node node);  // push a node
  
  void create_node(Node &parent, Record &record, std::string mode, int node_idx);  // create a node
  
  void create_nodes(Node& parent, Node& left, Node& right,
                    Record l_rec, Record r_rec);  // create left and right children
  void print_tree();        // print the tree
  
  //std::vector<int> GetLeafNodes(
  //    const std::vector<std::vector<double>> *feature_data_ptr);
  
  //void PopulateChildIds();
  
  //void PredictClassProbabilities();
  
  //std::vector<std::vector<double>> PredictClassProbabilities(
  //  const std::vector<std::vector<double>> *samples);
  
  //std::vector<int> PredictClasses(
  //    const std::vector<std::vector<double>> *feature_data_ptr);
};




#endif // DECISION_TREE_H_
