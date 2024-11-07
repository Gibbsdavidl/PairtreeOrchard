#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

#include <limits>
#include <vector>
#include <random>

//#include "splitter.h"

enum MaxFeaturesMethod { sqrt_method, log2_method, all_method };

struct Record {
  int start;
  int end;
  int n_samples;
  int depth;
  int parent_id;
  bool is_left;
  double impurity;
  std::vector<int> value;
};

struct NodeStack {
  std::vector<Record> stack;
  void push(Record record);
  void pop(Record &record);
  bool IsEmpty();
};

struct Node {
  int parent_id = 123;
  int left_child_id=234;
  int right_child_id=345;
  bool is_left=false;
  bool is_leaf=false;
  double impurity=0.0;
  int variable1=99;
  int variable2=88; // v1 < v2
  std::vector<int> split_feature;
  double proportion=0.0;  // of v1 < v2 / n_samples
  int n_samples=0;
  std::vector<int> value;   // what's this for?

  void Print();
};


struct DecisionTree {
  std::vector<Node> nodes;
  bool is_built = false;
  int AddNodeToDT(Node node);
  void Print();
  
  //std::vector<int> GetLeafNodes(
  //    const std::vector<std::vector<double>> *feature_data_ptr);
  
  //void PopulateChildIds();
  
  //void PredictClassProbabilities();
  
  //std::vector<std::vector<double>> PredictClassProbabilities(
  //  const std::vector<std::vector<double>> *samples);
  
  //std::vector<int> PredictClasses(
  //    const std::vector<std::vector<double>> *feature_data_ptr);
};



class DecisionTreeClassifier {
 public:

  DecisionTree tree_;
  
  NodeStack stack_;
  
  //Splitter splitter_;
  
  //ImpurityMeasure impurity_measure_;
  
  MaxFeaturesMethod max_features_method_;
  
  int min_samples_leaf_;
  
  int max_depth_;
  
  int min_samples_split_;
  
  double min_impurity_split_;
  
  std::size_t n_features_;
  
  std::size_t max_features_;
  
  std::mt19937 *gen_;
  
  std::mt19937 generator;
  
  std::vector<std::vector<double>> feature_data_;
  
  std::vector<int> label_data_;

  DecisionTreeClassifier();

  // DecisionTreeClassifier(//ImpurityMeasure impurity_measure = gini,
  //                        int max_depth = std::numeric_limits<int>::max(),
  //                        int min_samples_split = 2, 
  //                        int min_samples_leaf = 1,
  //                        int max_features = -1,
  //                        MaxFeaturesMethod max_features_method = all_method,
  //                        double min_impurity_split_ = 0.0);

  void PrintTree();

  void AddNode();

  //void BuildTree(const std::vector<std::vector<double>> *feature_data_ptr,
  //               const std::vector<int> *label_data_ptr,
  //               const std::vector<int> samples_subset, int n_labels,
  //               std::mt19937 *gen_);

  //void BuildTree(const std::vector<std::vector<double>> feature_data,
  //               const std::vector<int> label_data);
  
  //std::vector<int> PredictClasses(
  //    const std::vector<std::vector<double>> *feature_data_ptr);
};



#endif // tree builder
