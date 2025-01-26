#ifndef CRITERION_H_
#define CRITERION_H_

#include <cmath>
#include <iostream>
#include <vector>

enum ImpurityMeasure { gini, entropy };

struct SampleData {
  int sample_number;
  double current_feature_value;
};

class Criterion {

 public:
  const std::vector<std::vector<double>> *feature_data_;
  const std::vector<int> *label_data_;

  ImpurityMeasure impurity_measure_;
  const std::vector<SampleData> *sample_map_ptr_;
  double (*impurity_fn_)(std::vector<int> &, int &, int &);
  
  int start_;
  int end_;
  int pos_;
  int n_labels_;

  std::vector<int> label_freqs_total_;
  std::vector<int> label_freqs_left_;
  std::vector<int> label_freqs_right_;

  int n_samples_;
  int n_samples_left_;
  int n_samples_right_;

  //Criterion(ImpurityMeasure impurity_measure, int n_labels,
   //         const std::vector<std::vector<double>> *feature_data,
    //        const std::vector<int> *label_data);

  Criterion() {}
  
  void SetData(const std::vector<SampleData> *sample_map_ptr);
  void SetNodeLimits(int start, int end);
  void UpdateSplitPos(int new_pos);
  double ImpurityImprovement();
  double NodeImpurity();
  void ChildrenImpurities(double &impurity_left, double &impurity_right);
  void ResetStats();
  static double GiniCoefficient(std::vector<int> &label_freqs, int &n_samples,
                                int &n_labels);
  static double Entropy(std::vector<int> &label_freqs, int &n_samples,
                        int &n_labels);
};

#endif  // CRITERION_H_