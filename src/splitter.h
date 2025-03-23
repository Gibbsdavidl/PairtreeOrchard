#ifndef SPLITTER_H_
#define SPLITTER_H_

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "criterion.h"
#include "decision_tree.h"


struct Split {
  std::size_t feature;
  double threshold;
  int pos;
  double impurity_left;
  double impurity_right;
  std::vector<int> left_value;
  std::vector<int> right_value;
  bool found_split;

  void Print();
};


class Splitter {
 public:

  Criterion& criterion_;

  const std::vector<std::vector<double>>& feature_data_;
  const std::vector<int>& label_data_;
  const std::vector<int>* idx_;


  static constexpr double kMinSplitDiff_ = 1e-8;


  std::size_t n_samples_total_;
  std::size_t n_features_;


  int max_depth_;
  int min_samples_leaf_;
  int min_samples_split_;
  int max_features_;
  double min_label_entropy_;

  std::mt19937 *gen_;
  
  Splitter(const std::vector<std::vector<double>>& feature_data,
    const std::vector<int>& label_data,
    Criterion& criterion);
  
  void set_params(int max_depth,
    int min_samples_split, 
    int min_samples_leaf,
    int max_features,
    double min_impurity_split);

  void SetIdx(const std::vector<int>* idx);

  bool search_split(Node* curr);

  void print();

  void split(Node* curr, Record* l_rec, Record* r_rec); // Split &split
};

#endif  // SPLITTER_H_