#include "splitter.h"
#include "criterion.h"

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
            // Retrieve the logger created during module import
            logger_ = spdlog::get("pto");
            logger_->debug("Splitter instance created.");
    }


void Splitter::print() {
    std::cout << "Splitter: " << n_samples_total_ << std::endl;
 }

void Splitter::setParams(int max_depth,
    int min_samples_split, 
    int min_samples_leaf,
    int max_features,
    double min_label_entropy) {
        max_depth_=max_depth;
        min_samples_split_ = min_samples_split;
        min_samples_leaf_=min_samples_leaf;
        max_features_=max_features;
        min_label_entropy_ = min_label_entropy; 
        n_samples_total_=feature_data_.size();
        n_features_=feature_data_[0].size();
    }

void Splitter::extract_and_sort_data(
    int feature_index,
    std::vector<std::pair<double, int>> &paired)
{
    // clear out the paired object...
    paired.clear();
    // then for each item... create the pair
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

}


void Splitter::extract_data(
    int feature_index,
    std::vector<std::pair<double, int>> &paired)
{
    // clear out the paired object...
    paired.clear();
    // then for each item... create the pair
    for (int i : idx_) {
        double feature_val = feature_data_[i][feature_index];
        int label = label_data_[i];
        paired.emplace_back(feature_val, label);
    }

}


bool Splitter::c45_search_split(Node* curr) {

    std::cout << "Starting C4.5 split search for a node with {} samples. " << idx_.size() << std::endl;

    int best_feature = -1;
    double best_threshold = 0.0;
    double best_score = -100000.0;
    double parent_entropy = 0.0;

    std::vector<std::pair<double, int>> paired;  // extracted data and label

    std::vector<int> f_idx(n_features_);       // indices to features
    std::iota(f_idx.begin(), f_idx.end(), 0);  // Fill with 0, 1, ..., n_features-1

    std::vector<int> s_idx(idx_.size());       // indices to samples
    std::iota(s_idx.begin(), s_idx.end(), 0);  // Fill with 0, 1, ..., (n_samples in idx)-1

    for (int i : f_idx) {
        std::cout << "  (C45) Feature index: " << i << std::endl;

        // 1) Build (value,label) pairs for feature i, then sort ascending by value.
        extract_and_sort_data(i, paired);
        std::cout << "  (C45) data extracted and sorted" << std::endl;
        // After this call, `paired[j].first` is the j-th smallest feature‐value,
        // and `paired[j].second` is its corresponding class‐label.

        int N = static_cast<int>(paired.size());
        if (N < 2) {
            // Cannot split fewer than 2 samples, so skip.
            std::cout << "  (C45) cannot split less than 2 samples: " << std::endl;
            continue;
        }

        // ---------------------------------------------------------------------------
        // 2) Compute the “parent” entropy H(S) over ALL N samples of this feature.
        // ---------------------------------------------------------------------------
        std::unordered_map<int,int> parent_freq;
        for (int k = 0; k < N; ++k) {
            int lab = paired[k].second;
            parent_freq[lab] += 1;
        }
        parent_entropy = criterion_.entropy(parent_freq, N);

        std::cout << "  (C45) Parent entropy: " << parent_entropy << std::endl;

        // If parent entropy is zero, that means all labels are identical; not a candidate for splitting
        if (parent_entropy <= 0.0) {
            continue;
        }

        // --------------------------------------------------------
        // 3) Scan through adjacent pairs in “paired” to find ALL candidate thresholds.
        //    Only consider a split BETWEEN two sorted values if their labels differ.
        // --------------------------------------------------------
        for (int j = 1; j < N; ++j) {

            // Only consider a threshold if the labels actually change between paired[j-1] and paired[j].
            if (paired[j-1].second == paired[j].second) {
                std::cout << "  (C45) labels not flipping: " << j << std::endl;
                continue;  // same label → no “information boundary” here
            }

            // Candidate threshold is the midpoint between these two feature‐values:
            double v_left  = paired[j-1].first;
            double v_right = paired[j].first;
            double threshold = 0.5 * (v_left + v_right);

            std::cout << "  (C45) trying threshold: " << threshold << std::endl;

            // ----------------------------------------------------
            // 4) Partition into left‐subset S_L (value <= threshold)
            //    and right‐subset S_R (value > threshold), then
            //    compute entropies H(S_L), H(S_R) and sizes N_L, N_R.
            // ----------------------------------------------------
            std::unordered_map<int,int> left_freq, right_freq;
            int N_left = 0, N_right = 0;

            // Simple loop over ALL N to count how many go left/right
            for (int k = 0; k < N; ++k) {
                double feat_val = paired[k].first;
                int    lab      = paired[k].second;
                if (feat_val <= threshold) {
                    left_freq[lab] += 1;
                    ++N_left;
                } else {
                    right_freq[lab] += 1;
                    ++N_right;
                }
            }

            // If either side is empty, skip this threshold (no real split).
            if (N_left == 0 || N_right == 0) {
                continue;
            }

            double H_left  = criterion_.entropy(left_freq,  N_left);
            double H_right = criterion_.entropy(right_freq, N_right);

            // --------------------------------------------------------
            // 5) Compute Information Gain:
            //       IG = H(parent) - (N_L/N)*H_left - (N_R/N)*H_right
            // --------------------------------------------------------
            double wL = double(N_left) / double(N); // frac to the left
            double wR = double(N_right) / double(N); // frac to the right
            double info_gain = parent_entropy - (wL * H_left + wR * H_right);

            std::cout << "  (C45) info gain: " << info_gain << std::endl;

            // ------------------------------------------------------------------
            // 6) Compute Split Information:
            //       SI = - (N_L/N)*log2(N_L/N)  -  (N_R/N)*log2(N_R/N)
            // ------------------------------------------------------------------
            double split_info = 0.0;
            // note: we know N_left>0 and N_right>0, so wL>0, wR>0
            split_info -= wL * std::log2(wL);
            split_info -= wR * std::log2(wR);

            // If split‐info is zero (unlikely unless one side empty, but we checked),
            // then gain ratio is undefined → skip.
            if (split_info <= 0.0) {
                continue;
            }

            // --------------------------------------------------------
            // 7) Compute Gain Ratio = IG / split_info
            // --------------------------------------------------------
            double gain_ratio = info_gain / split_info;

            std::cout << "  (C45) gain ratio: " << gain_ratio << std::endl;


            // --------------------------------------------------------
            // 8) If this is the best we’ve seen so far, record it.
            // --------------------------------------------------------
            if (gain_ratio > best_score) {
                best_score     = gain_ratio;
                best_feature   = i;
                best_threshold = threshold;
            }
        }  // end loop over j = [1 … N-1]

    }
    // ---------------------------------------------------------------------
    // 9) At this point, best_feature and best_threshold hold the overall best split.
    //    If no valid split was found, best_feature will remain -1.
    // ---------------------------------------------------------------------
    if (best_feature < 0) {
        // No split improved gain‐ratio → do not split this node
        return false;
    }

    // Otherwise, attach the best split information to the current node:
    curr->record_.variable1_ = best_feature;
    curr->record_.threshold  = best_threshold;
    curr->record_.gain_ratio = best_score;
    curr->record_.entropy_   = parent_entropy;
    return true;
}


bool Splitter::pt_search_split(Node* curr) {
    return false;
}


bool Splitter::searchSplit(Node* curr, std::string split_mode) {

    std::cout << "in searchSplit" << std::endl;

    // check if we should split or whether it's a leaf.

    // IDEA: instead of a paired comparison,
    // make the pair-partner a threshold T, then
    // it's like a traditional decision tree?

    // 1. Calculate the REAL entropy first
    double current_entropy = criterion_.calculateEntropy(curr->record_.index_);
    
    // 2. Update the node's record
    curr->record_.entropy_ = current_entropy;


    if (curr->record_.n_samples_ < min_samples_split_) {
        logger_->debug("n_samples {} less than min_samples_split: {}.", curr->record_.n_samples_, min_samples_split_);
        return false;
    }
    if (curr->record_.entropy_ < min_label_entropy_) {
        logger_->debug("entropy {} less than min_label_entropy: {}.", curr->record_.entropy_, min_label_entropy_);
        return false;
    }
    if (curr->depth_ >= max_depth_) {
        logger_->debug("depth {} gt than max_depth_: {}.", curr->depth_, max_depth_);
        return false;
    }
    // traditional decision tree, dt
    if (split_mode == "c45") {
        // then compare splits to a given threshold
        return c45_search_split(curr);
    }

    if (split_mode == "pt") {
        // or grow a pairtree
        return pt_search_split(curr);
    }

    // set in curr the best split, which vars are used
    return false;
}



void Splitter::c45_split(Node* curr, Record* l_rec, Record* r_rec){

    // read the curr node
    Record curr_record = curr->record_;
    int feature = curr_record.variable1_;
    double threshold = curr_record.threshold;

    // build the index to L and R

    std::vector<std::pair<double, int>> paired;  // extracted data and label
    extract_and_sort_data(feature, paired);    // sort by best
    int N = static_cast<int>(paired.size());
    // After this call, `paired[j].first` is the j-th smallest feature‐value,
    // and `paired[j].second` is its corresponding class‐label.

    std::vector<int> left_idx, right_idx; // index and label //
    std::unordered_map<int,int> left_freq, right_freq;
    int N_left = 0, N_right = 0;

    // Simple loop over ALL N to count how many go left/right
    for (int k = 0; k < N; ++k) {
        double feat_val = paired[k].first;
        int    lab      = paired[k].second;
        if (feat_val <= threshold) {
            left_idx.emplace_back( k );
            left_freq[lab] += 1;
            ++N_left;
        } else {
            right_idx.emplace_back( k );
            right_freq[lab] += 1;
            ++N_right;
        }
    }

    double H_left  = criterion_.entropy(left_freq,  N_left);
    double H_right = criterion_.entropy(right_freq, N_right);

    // and fill in the records for the left and right nodes
    // based on saved split info from search split
    l_rec->n_samples_ = N_left;
    l_rec->entropy_   = H_left;
    l_rec->index_     = left_idx;
    l_rec->threshold  = threshold;
    l_rec->variable1_ = feature;
    l_rec->variable2_ = -1;
    l_rec->gain_ratio = 0.0;

    r_rec->n_samples_ = N_left;
    r_rec->entropy_   = H_left;
    r_rec->index_     = left_idx;
    r_rec->threshold  = threshold;
    r_rec->variable1_ = feature;
    r_rec->variable2_ = -1;
    r_rec->gain_ratio = 0.0;

    return;
}



void Splitter::split(Node* curr, Record* l_rec, Record* r_rec, std::string split_mode) { 

    if (split_mode == "c45") {
        // then compare splits to a given threshold
        return c45_split(curr, l_rec, r_rec);
    }


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