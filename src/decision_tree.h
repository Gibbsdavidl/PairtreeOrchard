#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

#include <limits>
#include <vector>
#include <random>
#include <string>

//#include "splitter.h"

enum MaxFeaturesMethod { sqrt_method, log2_method, all_method };

struct Record {         // about the data contained in the node
  int n_samples_=0;      // number of samples in the set
  double entropy_=0.0;   // entropy of this node
  std::vector<int> index_;  // index in to data and labels
  int variable1_=0;
  int variable2_=0; // v1 > v2
  double prob_v1gtv2_=0.0;  // of v1 > v2 / n_samples
  double threshold=0.0;
  double gain_ratio = 0.0;

};


struct Node {       // about the node in the tree
  int depth_=0;          // depth in the tree
  int parent_id_=0;
  int self_id_=0;
  int left_child_id_=0;
  int right_child_id_=0;
  bool is_left_=false;
  bool is_leaf_=false;
  Record record_;
  void printNode();
};


struct NodeStack {
  std::vector<Node> stack_;
  void push(Node record);
  void pop(Node &record);
  bool isEmpty();
};


struct RecordStack {
  std::vector<Record> stack_;
  void push(Record record);
  void pop(Record &record);
  bool isEmpty();
};


struct DecisionTree {

  std::vector<Node> nodes_;  // tree stored as vector of nodes

  bool is_built_ = false;    // is the tree constructed yet?

  int addNode(Node node);  // push a node
  
  void createNode(Node &parent, Record &record, std::string mode, int node_idx);  // create a node
  
  void createNodes(Node& parent, Node& left, Node& right,
                    Record l_rec, Record r_rec);  // create left and right children
  void printTree();        // print the tree
  
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
