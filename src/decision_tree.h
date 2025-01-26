#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

#include <limits>
#include <vector>
#include <random>

//#include "splitter.h"

enum MaxFeaturesMethod { sqrt_method, log2_method, all_method };


struct Record {
  int start=0;
  int end=0;
  int n_samples=0;
  int depth=0;
  int parent_id=0;
  bool is_left=0;
  double impurity=0.0;
  std::vector<int> value;
};


struct NodeStack {
  std::vector<Record> stack;
  void push(Record record);
  void pop(Record &record);
  bool is_empty();
};


struct Node {
  int parent_id=0;
  int left_child_id=0;
  int right_child_id=0;
  bool is_left=false;
  bool is_leaf=false;
  double impurity=0.0;
  int variable1=0;
  int variable2=0; // v1 < v2
  std::vector<int> split_feature;
  double proportion=0.0;  // of v1 < v2 / n_samples
  int n_samples=0;
  std::vector<int> value;   // what's this for?

  void print_node();
};


struct DecisionTree {
  std::vector<Node> nodes;
  bool is_built = false;
  int add_node(Node node);
  void print_tree();
  
  //std::vector<int> GetLeafNodes(
  //    const std::vector<std::vector<double>> *feature_data_ptr);
  
  //void PopulateChildIds();
  
  //void PredictClassProbabilities();
  
  //std::vector<std::vector<double>> PredictClassProbabilities(
  //  const std::vector<std::vector<double>> *samples);
  
  //std::vector<int> PredictClasses(
  //    const std::vector<std::vector<double>> *feature_data_ptr);
};




#endif // tree builder
