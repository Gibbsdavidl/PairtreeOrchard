#ifndef CRITERION_H_
#define CRITERION_H_

#include <cmath>
#include <iostream>
#include <vector>

enum InformationMeasure { entropy, gini };

struct SampleData {
  int sample_number;
  double current_feature_value;
};

class Criterion {

 public:

  InformationMeasure information_measure_;
  const std::vector<std::vector<double>> *feature_data_;
  const std::vector<int> *label_data_;

  const std::vector<SampleData> *sample_map_ptr_;
  double (*impurity_fn_)(std::vector<int> &, int &);

  std::vector<int> label_freqs_total_;
  std::vector<int> label_freqs_left_;
  std::vector<int> label_freqs_right_;

  int n_samples_;
  int n_samples_left_;
  int n_samples_right_;

  Criterion(InformationMeasure information_measure, 
           const std::vector<std::vector<double>> *feature_data,
           const std::vector<int> *label_data,
           const std::vector<int> *sample_index);

  Criterion() {}
  
  void SetIndex(const std::vector<SampleData> *sample_map_ptr);
  void UpdateSplitPos(int new_pos);
  double InformationGain();
  double NodeInformation();
  void ChildrenInformation(double &impurity_left, double &impurity_right);
  void ResetStats();
  static double GiniCoefficient(std::vector<int> &label_freqs, int &n_samples);
  static double Entropy(std::vector<int> &label_freqs, int &n_samples);
};

#endif  // CRITERION_H_