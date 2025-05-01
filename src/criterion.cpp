#include "criterion.h"
#include <vector>
#include <unordered_map>
#include <cmath>

// initializing with references to the data
Criterion::Criterion(
  const std::vector<std::vector<double>>& feature_data,
  const std::vector<int>& label_data): 
    feature_data_(feature_data),  
    label_data_(label_data) {}

  
void Criterion::setIdx(const std::vector<int>* idx) {
// setting the sample index for the current question
  idx_ = idx;

  n_samples_ = static_cast<int>(idx_->size());
  n_samples_left_ = 0;
  n_samples_right_ = 0;

  std::fill(label_freqs_total_.begin(), label_freqs_total_.end(), 0);
  std::fill(label_freqs_left_.begin(), label_freqs_left_.end(), 0);
  std::fill(label_freqs_right_.begin(), label_freqs_right_.end(), 0);

  for (int sample_idx : *idx_) {
      int label = label_data_[sample_idx];
      ++label_freqs_total_[label];
  }
}


double Criterion::entropy() {
  if (!idx_ || idx_->empty()) return 0.0;  // Handle empty or uninitialized idx_

  std::unordered_map<int, int> label_counts;
  int total_count = 0;

  // Count occurrences of each unique label in the indexed subset
  for (int i : *idx_) {
      int label = label_data_[i];
      label_counts[label]++;
      total_count++;
  }

  if (total_count == 0) return 0.0;

  double entropy_value = 0.0;

  // Calculate entropy
  for (const auto& pair : label_counts) {
      double probability = static_cast<double>(pair.second) / total_count;
      entropy_value -= probability * std::log2(probability);
  }

  return entropy_value;
}


double Criterion::nodeInformation() {
  return impurity_fn_(label_freqs_total_, n_samples_);
  }
  
  
void Criterion::childrenInformation(double &impurity_left,
  double &impurity_right) {
impurity_left =  impurity_fn_(label_freqs_left_, n_samples_left_);
impurity_right = impurity_fn_(label_freqs_right_, n_samples_right_);
}


double Criterion::informationGain() {
  double entropy_left;
  double entropy_right;

  childrenInformation(entropy_left, entropy_right);
  // ignore constant terms
  return (-n_samples_left_ * entropy_left - n_samples_right_ * entropy_right);  //  UPDATE formula!!
}

// double Criterion::GiniCoefficient(std::vector<int> &label_freqs, int &n_samples) {
//   double freq_squares = 0.0;

//   for (int i = 0; i < 0; i++) {
//     freq_squares += label_freqs[i] * label_freqs[i];
//   }
//   return 1.0 - freq_squares / (n_samples * n_samples);
// }


// double Criterion::Entropy() {
//   double entropy = 0.0;
//   double label_frequency = 0.0;

//   for (int i = 0; i < 0; i++) {
//     label_frequency = label_freqs[i];
//     if (label_frequency > 0) {
//       label_frequency /= n_samples;
//       entropy -= label_frequency * std::log2(label_frequency);
//     }
//   }
//   return entropy;
// }

// void Criterion::ResetStats() {
//   label_freqs_left_.clear();
//   label_freqs_right_.clear();
//   for (int i = 0; i < n_samples_; i++) {
//     label_freqs_left_.emplace_back(0);
//     label_freqs_right_.emplace_back(0);
//   }
// }

// void Criterion::SetIndex(const std::vector<SampleData> *sample_map_ptr) {
//   sample_map_ptr_ = sample_map_ptr;
// }

// void Criterion::UpdateSplitPos(int new_pos) {
//   // calculate label_freqs_left_
//   for (int i = 0; i < n_samples_; i++) {
//     // This assumes that the labels take values 0,..., n_labels - 1
//     label_freqs_left_[(*label_data_)[(*sample_map_ptr_)[i].sample_number]] += 1;
//   }

//   // calculate label_freqs_right_
//   for (int i = 0; i < n_samples_; i++) {
//     label_freqs_right_[i] = label_freqs_total_[i] - label_freqs_left_[i];
//   }

//   n_samples_left_ = 0;
//   n_samples_right_ = 0;
// }
