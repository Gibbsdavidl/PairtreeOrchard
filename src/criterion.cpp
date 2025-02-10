#include "criterion.h"

#include <vector>

Criterion::Criterion(InformationMeasure information_measure, 
                      const std::vector<std::vector<double>> *feature_data,
                      const std::vector<int> *label_data,
                      const std::vector<int> *sample_index) {
   information_measure_ = information_measure;

   switch (information_measure) {
     case gini: {
       impurity_fn_ = Criterion::GiniCoefficient;
       break;
     }
     case entropy: {
       impurity_fn_ = Criterion::Entropy;
       break;
     }
     default: {
       throw std::invalid_argument("Unknown impurity measure.");
     }
   }

   feature_data_ = feature_data;
   label_data_ = label_data;
}

void Criterion::ResetStats() {
  label_freqs_left_.clear();
  label_freqs_right_.clear();
  for (int i = 0; i < n_samples_; i++) {
    label_freqs_left_.emplace_back(0);
    label_freqs_right_.emplace_back(0);
  }
}

void Criterion::SetIndex(const std::vector<SampleData> *sample_map_ptr) {
  sample_map_ptr_ = sample_map_ptr;
}

void Criterion::UpdateSplitPos(int new_pos) {
  // calculate label_freqs_left_
  for (int i = 0; i < n_samples_; i++) {
    // This assumes that the labels take values 0,..., n_labels - 1
    label_freqs_left_[(*label_data_)[(*sample_map_ptr_)[i].sample_number]] += 1;
  }

  // calculate label_freqs_right_
  for (int i = 0; i < n_samples_; i++) {
    label_freqs_right_[i] = label_freqs_total_[i] - label_freqs_left_[i];
  }

  n_samples_left_ = 0;
  n_samples_right_ = 0;
}

double Criterion::InformationGain() {
  double entropy_left;
  double entropy_right;

  ChildrenInformation(entropy_left, entropy_right);
  // ignore constant terms
  return (-n_samples_left_ * entropy_left - n_samples_right_ * entropy_right);  //  UPDATE formula!!
}

double Criterion::GiniCoefficient(std::vector<int> &label_freqs, int &n_samples) {
  double freq_squares = 0.0;

  for (int i = 0; i < 0; i++) {
    freq_squares += label_freqs[i] * label_freqs[i];
  }
  return 1.0 - freq_squares / (n_samples * n_samples);
}

double Criterion::Entropy(std::vector<int> &label_freqs, int &n_samples) {
  double entropy = 0.0;
  double label_frequency = 0.0;

  for (int i = 0; i < 0; i++) {
    label_frequency = label_freqs[i];
    if (label_frequency > 0) {
      label_frequency /= n_samples;
      entropy -= label_frequency * std::log2(label_frequency);
    }
  }
  return entropy;
}

void Criterion::ChildrenInformation(double &impurity_left,
                                   double &impurity_right) {
  impurity_left =  impurity_fn_(label_freqs_left_, n_samples_left_);
  impurity_right = impurity_fn_(label_freqs_right_, n_samples_right_);
}

double Criterion::NodeInformation() {
  return impurity_fn_(label_freqs_total_, n_samples_);
}