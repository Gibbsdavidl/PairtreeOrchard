#ifndef DECISION_TREE_CLASSIFIER_H_
#define DECISION_TREE_CLASSIFIER_H_

#include <limits>
#include <vector>
#include <random>

#include "decision_tree.h"
#include "criterion.h"
#include "splitter.h"

class DecisionTreeClassifier {

 public:

  DecisionTree tree_;    // the tree structure
  NodeStack stack_;      // stack of nodes

  std::vector<std::vector<double>> feature_data_;
  std::vector<int> label_data_;

  InformationMeasure information_measure_;    // what measure to use?
  //MaxFeaturesMethod max_features_method_;   // number of features 

  // parameters related to building the tree
  std::size_t max_features_;  // 
  int min_samples_leaf_;      // when to stop splitting due to few samples
  int max_depth_;             // maximum depth of the tree, when to stop
  int min_samples_split_;     // number of samples needed to split?
  double min_impurity_split_; // 

  std::size_t n_samples_;    // number of features present in data
  std::size_t n_features_;    // number of features present in data
  std::mt19937 *gen_;
  std::mt19937 generator_;

  DecisionTreeClassifier();  // default settings

  DecisionTreeClassifier(int max_depth,
                         int min_samples_split, 
                         int min_samples_leaf,
                         int max_features,
                         double min_impurity_split);

  void print();

  Record initRecord(int n_samples);

  void buildTree(const std::vector<std::vector<double>> feature_data,
                 const std::vector<int> label_data);
  
  //std::vector<int> PredictClasses(
  //    const std::vector<std::vector<double>> *feature_data_ptr);
};

#endif // DECISION_TREE_CLASSIFIER_H_