#include "splitter.h"

#include <limits>
#include <vector>


Splitter::Splitter(
    const std::vector<std::vector<double>>& feature_data,
        const std::vector<int>& label_data,
        Criterion& criterion,
        const std::vector<int>& idx): 
        feature_data_(feature_data),  
        label_data_(label_data),
        criterion_(criterion),
        idx_(idx) {
    }


void Splitter::print() {
    std::cout << "Splitter: " << n_samples_total_ << std::endl;
 }

void Splitter::setParams(int max_depth,
    int min_samples_split, 
    int min_samples_leaf,
    int max_features,
    double min_impurity_split) {
        max_depth_=max_depth;
        min_samples_leaf_=min_samples_leaf;
        max_features_=max_features;
        n_samples_total_=feature_data_.size();
        n_features_=feature_data_[0].size();
    }

void Splitter::extract_and_sort_data(
    int feature_index,
    std::vector<std::pair<double, int>> paired)
    //std::vector<double>& sorted_feature_values,
    //std::vector<int>& sorted_labels)
{
    for (int i : idx_) {
        double feature_val = feature_data_[i][feature_index];
        int label = label_data_[i];
        paired.emplace_back(feature_val, label);
    }

    // Sort the pairs by feature value
    std::sort(paired.begin(), paired.end(),
              [](const std::pair<double, int>& a, const std::pair<double, int>& b) {
                  return a.first < b.first;
              });

    // // Unpack sorted values and labels into output vectors
    // sorted_feature_values.clear();
    // sorted_labels.clear();
    // for (const auto& p : paired) {
    //     sorted_feature_values.push_back(p.first);
    //     sorted_labels.push_back(p.second);
    // }
}

bool Splitter::c45_split(Node* curr) {

    std::cout << "in c45 split" << std::endl;

    int best_feature = -1;
    double best_threshold = 0.0;
    double best_score = -100000.0;

    std::vector<std::pair<double, int>> paired;  // extracted data and label

    std::vector<int> f_idx(n_features_);       // indices to features
    std::iota(f_idx.begin(), f_idx.end(), 0);  // Fill with 0, 1, ..., n_features-1

    std::vector<int> s_idx(idx_.size());       // indices to samples
    std::iota(s_idx.begin(), s_idx.end(), 0);  // Fill with 0, 1, ..., (n_samples in idx)-1

    for (int i : f_idx) {
        std::cout << "Feature index: " << i << std::endl;

        extract_and_sort_data(i, paired); // now paired has <value, label> for feature i

        // we are going to check splits where the label changes sign.
        // for each possible split (value or threshold) of f:
        for (int j : s_idx) {

            std::cout << "feature " << i << " sample " << j << paired[j].first  << "  " << paired[j].second << std::endl;

        }
    }


    //         partition data into subsets S1, S2, ...
    //         compute entropy/information gain (ID3) or gain ratio (C4.5/C5)
    //         if score > best_score:
    //             best_score = score
    //             best_feature = f
    //             best_threshold = value
    // return best_feature, best_threshold


    return false;
}


bool Splitter::pt_split(Node* curr) {
    return false;
}


bool Splitter::searchSplit(Node* curr, std::string split_mode) {

    std::cout << "in search split" << std::endl;

    // check if we should split or whether it's a leaf.

    // IDEA: instead of a paired comparison,
    // make the pair-partner a threshold T, then
    // it's like a traditional decision tree?
    if (curr->record_.n_samples_ < min_samples_split_) {
        return false;
    }
    if (curr->record_.entropy_ < min_label_entropy_) {
        return false;
    }
    if (curr->depth_ >= max_depth_) {
        return false;
    }
    // traditional decision tree, dt
    if (split_mode == "c45") {
        // then compare splits to a given threshold
        return c45_split(curr);
    }

    if (split_mode == "pt") {
        // or grow a pairtree
        return pt_split(curr);
    }

    // set in curr the best split, which vars are used
    return false;
}


void Splitter::split(Node* curr, Record* l_rec, Record* r_rec) { 

    // read the curr node

    // and fill in the records for the left and right nodes

    // based on saved split info from search split
}


// Splitter::Splitter(const std::vector<std::vector<double>> *feature_data,
//                    const std::vector<int> *label_data, 
//                    int min_samples_leaf,
//                    InformationMeasure impurity_measure, std::size_t max_features,
//                    int n_labels, std::mt19937 *gen,
//                    const std::vector<int> &samples_subset) {
//   feature_data_ = feature_data;
//   label_data_ = label_data;
//   n_features_ = (*feature_data)[0].size();
//   min_samples_leaf_ = min_samples_leaf;
//   max_features_ = max_features;
//   gen_ = gen;

//   shuffle_features_ = false;
//   if (max_features_ < (*feature_data)[0].size()) {
//     shuffle_features_ = true;
//   }

//   if (samples_subset.size() == 0) {
//     n_samples_total_ = (*feature_data).size();
//     sample_map_.reserve(n_samples_total_);
//     for (std::size_t i = 0; i < n_samples_total_; i++) {
//       SampleData sample;
//       sample.current_feature_value = 0.0;
//       sample.sample_number = i;
//       sample_map_.push_back(sample);
//     }
//   } else {
//     n_samples_total_ = samples_subset.size();
//     sample_map_.reserve(n_samples_total_);
//     for (std::size_t i = 0; i < n_samples_total_; i++) {
//       SampleData sample;
//       sample.current_feature_value = 0.0;
//       sample.sample_number = samples_subset[i];
//       sample_map_.push_back(sample);
//     }
//   }
//   criterion_ =
//       Criterion();

//   for (std::size_t i = 0; i < n_features_; i++) {
//     feature_order_.emplace_back(i);
//   }
// }


// void Split::Print() {
//   std::cout << "Feature: " << feature << std::endl;
//   std::cout << "Threshold: " << threshold << std::endl;
//   std::cout << "Split position: " << pos << std::endl;
//   std::cout << "Impurity left: " << impurity_left << std::endl;
//   std::cout << "Impurity right: " << impurity_right << std::endl;
// }



// void Splitter::ResetSampleRange(int start, int end) {
//   // start_ inclusive and end_ exclusive
//   start_ = start;
//   end_ = end;
//   //criterion_.SetNodeLimits(start_, end_);
// }




// void Splitter::SplitNode(Split &split) {
//   Split current_split;
//   Split best_split;
//   double current_impurity_improvement =
//       -std::numeric_limits<double>::infinity();
//   double best_impurity_improvement = -std::numeric_limits<double>::infinity();
//   std::size_t current_feature_num = 0;
//   std::size_t current_feature = 0;
//   current_split.found_split = false;

//   if (shuffle_features_) {
//     std::shuffle(feature_order_.begin(), feature_order_.end(), *gen_);
//   }

//   while (current_feature_num < max_features_) {
//     current_feature = feature_order_[current_feature_num];
//     current_split.feature = current_feature;
//     // set current feature values for the samples in the current node
//     for (int i = start_; i < end_; i++) {
//       sample_map_[i].current_feature_value =
//           (*feature_data_)[sample_map_[i].sample_number][current_feature];
//     }
//     criterion_.ResetStats();

//     // sort sample_map by current feature values
//     std::sort(sample_map_.begin() + start_, sample_map_.begin() + end_,
//               [](SampleData const &a, SampleData const &b) {
//                 return a.current_feature_value < b.current_feature_value;
//               });

//     if (sample_map_[start_].current_feature_value ==
//         sample_map_[end_ - 1].current_feature_value) {
//       current_feature_num++;
//       continue;
//     }

//     // loop through possible split positions
//     int pos = start_ + 1;
//     // split left [0, pos - 1] right [pos, end_-1]
//     while (pos < end_) {
//       // skip criterion evaluation for splits with less that kMinSplitDiff
//       // difference across split + kMinSplitDiff_
//       while (pos < end_ &&
//              sample_map_[pos].current_feature_value <=
//                  sample_map_[pos - 1].current_feature_value + kMinSplitDiff_) {
//         pos++;
//       }
//       if (pos == end_) {
//         pos++;
//         continue;
//       }

//       // check if split would lead to less than min_samples_leaf samples
//       if (!((pos - start_) < min_samples_leaf_ ||
//             ((end_ - pos) < min_samples_leaf_))) {
//         current_split.pos = pos;
//         criterion_.UpdateSplitPos(current_split.pos);
//         current_impurity_improvement = criterion_.InformationGain();

//         if (current_impurity_improvement > best_impurity_improvement) {
//           best_impurity_improvement = current_impurity_improvement;
//           current_split.found_split = true;
//           current_split.threshold =
//               (sample_map_[pos - 1].current_feature_value +
//                sample_map_[pos].current_feature_value) /
//               2.0;
//           best_split = current_split;
//           best_split.left_value = criterion_.label_freqs_left_;
//           best_split.right_value = criterion_.label_freqs_right_;
//         }
//       }
//       pos++;
//     }

//     current_feature_num++;
//   }

//   if (current_split.found_split) {
//     if (best_split.pos < end_) {
//       if (current_feature != best_split.feature) {
//         int left_pos = start_;
//         int right_pos = end_;
//         int tmp = 0;

//         while (left_pos < right_pos) {
//           if ((*feature_data_)[sample_map_[left_pos].sample_number]
//                               [best_split.feature] <= best_split.threshold) {
//             left_pos++;
//           } else {
//             right_pos--;
//             tmp = sample_map_[left_pos].sample_number;
//             sample_map_[left_pos].sample_number =
//                 sample_map_[right_pos].sample_number;
//             sample_map_[right_pos].sample_number = tmp;
//           }
//         }
//       }
//     }

//     criterion_.ResetStats();
//     criterion_.UpdateSplitPos(best_split.pos);
//     criterion_.ChildrenInformation(best_split.impurity_left,
//                                   best_split.impurity_right);
//     split = best_split;
//   } else {
//     // passing back split.found_split = false
//     split = current_split;
//   }
// }