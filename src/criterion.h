#ifndef CRITERION_H_
#define CRITERION_H_

#include <cmath>
#include <iostream>
#include <vector>
#include <unordered_map>

enum InformationMeasure { ENTROPY, GINI };

struct SampleData {
  int sample_number;
  double current_feature_value;
};

class Criterion {

 public:

  const std::vector<std::vector<double>>& feature_data_;
  const std::vector<int>& label_data_;
  const std::vector<int>* idx_;

  InformationMeasure information_measure_;
  double (*impurity_fn_)(std::vector<int> &, int &);

  std::vector<int> label_freqs_total_;
  std::vector<int> label_freqs_left_;
  std::vector<int> label_freqs_right_;

  int n_samples_;
  int n_samples_left_;
  int n_samples_right_;


  Criterion(
    const std::vector<std::vector<double>>& feature_data,
    const std::vector<int>& label_data);

  void setIdx(const std::vector<int>* idx);

  double entropy(const std::unordered_map<int,int>&, int);

  double entropy();

  double nodeInformation();

  void childrenInformation(double &impurity_left, double &impurity_right);

  double informationGain();


  //static double GiniCoefficient(std::vector<int> &label_freqs, int &n_samples);
  //void SetIndex(const std::vector<SampleData> *sample_map_ptr);
  //void UpdateSplitPos(int new_pos);
  //void ResetStats();
};

#endif  // CRITERION_H_