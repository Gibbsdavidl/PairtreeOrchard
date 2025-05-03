#ifndef SPLITTER_H_
#define SPLITTER_H_

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "criterion.h"
#include "decision_tree.h"


struct Split {
  std::size_t feature_;
  double threshold_;
  int pos_;
  double impurity_left_;
  double impurity_right_;
  std::vector<int> left_value_;
  std::vector<int> right_value_;
  bool found_split_;

  void Print();
};


class Splitter {
 public:

  Criterion& criterion_;

  const std::vector<std::vector<double>>& feature_data_;
  const std::vector<int>& label_data_;
  const std::vector<int>& idx_;

  std::vector<int> vdx_;  // selected variables used in splitting

  static constexpr double k_min_split_diff_ = 1e-8;

  std::size_t n_samples_total_;
  std::size_t n_features_;

  int max_depth_;
  int min_samples_leaf_;
  int min_samples_split_;
  int max_features_;
  double min_label_entropy_;

  std::mt19937 *gen_;

  Splitter(
    const std::vector<std::vector<double>>& feature_data,
    const std::vector<int>& label_data,
    Criterion& criterion,
    const std::vector<int>& idx);
  
  void setParams(
    int max_depth,
    int min_samples_split, 
    int min_samples_leaf,
    int max_features,
    double min_label_entropy_);

  bool searchSplit(Node* curr, std::string split_mode);

  void print();

  void split(Node* curr, Record* l_rec, Record* r_rec); // Split &split

  void extract_and_sort_data(int feature_j, std::vector<std::pair<double, int>> paired);

  bool c45_split(Node* curr);

  bool pt_split(Node* curr);
};

#endif  // SPLITTER_H_