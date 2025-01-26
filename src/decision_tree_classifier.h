#ifndef DECISION_TREE_CLASSIFIER_H_
#define DECISION_TREE_CLASSIFIER_H_

#include <limits>
#include <vector>
#include <random>

#include "decision_tree.h"

class DecisionTreeClassifier {

 public:

  DecisionTree tree_;    // the tree structure
  NodeStack stack_;      //
  std::vector<std::vector<double>> feature_data_;
  std::vector<int> label_data_;
  
  //Splitter splitter_;  // finds the split
  //ImpurityMeasure impurity_measure_;      // calculates the measure
  MaxFeaturesMethod max_features_method_;   // number of features 
  std::size_t max_features_;  // 
  int min_samples_leaf_;      // when to stop splitting due to few samples
  int max_depth_;             // maximum depth of the tree, when to stop
  int min_samples_split_;     // number of samples needed to split?
  double min_impurity_split_; // 
  std::size_t n_features_;    // number of features present in data
  std::mt19937 *gen_;
  std::mt19937 generator;


  DecisionTreeClassifier(); // default classifier

  DecisionTreeClassifier(int max_depth,
                         int min_samples_split, 
                         int min_samples_leaf,
                         int max_features,
                         double min_impurity_split_);

  void print();

  void load_data(
      const std::vector<int> label_data,
      const std::vector<std::vector<double>> feature_data);

  void add_node();

  //void BuildTree(const std::vector<std::vector<double>> *feature_data_ptr,
  //               const std::vector<int> *label_data_ptr,
  //               const std::vector<int> samples_subset, int n_labels,
  //               std::mt19937 *gen_);

  //void BuildTree(const std::vector<std::vector<double>> feature_data,
  //               const std::vector<int> label_data);
  
  //std::vector<int> PredictClasses(
  //    const std::vector<std::vector<double>> *feature_data_ptr);
};

#endif // DECISION_TREE_CLASSIFIER_H_