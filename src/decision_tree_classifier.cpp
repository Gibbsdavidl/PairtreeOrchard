#include "decision_tree_classifier.h"
#include "splitter.h"
#include <iostream>
#include <vector>



// DT Classifier definitions

std::vector<int> createVectorFromIToJ(int i, int j) {
    std::vector<int> vec;
    for (int k = i; k <= (j-1); ++k) {
        vec.push_back(k);
    }
    return vec;
}

DecisionTreeClassifier::DecisionTreeClassifier(): 
criterion_(feature_data_, label_data_),  
splitter_(feature_data_, label_data_, criterion_)  
{}


DecisionTreeClassifier::DecisionTreeClassifier(
    int max_depth,
    int min_samples_split,
    int min_samples_leaf,
    int max_features,
    double min_impurity_split
  ): 
    criterion_(feature_data_, label_data_),  
    splitter_(feature_data_, label_data_, criterion_)  
  {

    max_depth_ = max_depth;
    min_samples_split_ = min_samples_split;
    min_samples_leaf_ = min_samples_leaf;
    max_features_ = max_features;
    min_impurity_split_ = min_impurity_split;  

    splitter_.set_params(max_depth_,
                         min_samples_split_,
                         min_samples_leaf_,
                         max_features_,
                         min_impurity_split_);
  }


void DecisionTreeClassifier::print() { 
  std::cout << "\nDT Classifier" << std::endl;
  std::cout << "Max Depth: " << max_depth_ << std::endl;
  std::cout << "Decision Tree:" << std::endl;
  tree_.print_tree(); 
}


void DecisionTreeClassifier::BuildTree(
              const std::vector<std::vector<double>> feature_data,
              const std::vector<int> label_data) {

    label_data_ = label_data;
    feature_data_ = feature_data;

    n_samples_ = label_data_.size();
    n_features_ = feature_data.size();

    Criterion criterion_ = Criterion(feature_data, label_data);
    Splitter splitter_ = Splitter(feature_data, label_data, criterion_);

    // assert
    // length of one row of feature_data must equal n_samples_

    // assert
    // labels must be 0s and 1s

    // check
    // that labels have both 0s and 1s

    std::cout << "label length: " << n_samples_ << std::endl;
    std::cout << "features rows: " << n_features_ << std::endl;
    std::cout << "features cols: " << feature_data_[0].size() << std::endl;

    // start with root node
    Record curr_record = Record();
    curr_record.n_samples=n_samples_;    // number of samples in the set
    curr_record.index=createVectorFromIToJ(0, n_samples_);   // index in to data and labels
    curr_record.entropy=0.0;       // entropy of this node, updated at search_split
    curr_record.variable1=-1;        // index into feature rows
    curr_record.variable2=-1;        // used for pair comparison v1 > v2
    curr_record.prob_v1gtv2=0.0;  // of v1 > v2 / n_samples

    // create root node, contains the root record
    int node_index = 0;
    Node parent_node = Node();
    std::string curr_mode = "root";
    tree_.create_node(parent_node, curr_record, curr_mode, node_index); 
    tree_.add_node(parent_node);

    //  push current to stack
    stack_.push(parent_node);

    //  pop stack
    while (!stack_.is_empty()) {

        //    pop current
        Node curr_node = Node();
        stack_.pop(curr_node);
        std::cout << "popped: " << curr_node.record_.n_samples << std::endl;

        // update criterion with curr index
        splitter_.SetIdx(&curr_node.record_.index);

        //    if current criteria says to split
        if (splitter_.search_split(&curr_node) == false) {
            //    split node
            Record l_rec;
            Record r_rec;   // fills in the records
            splitter_.split(&curr_node, &l_rec, &r_rec);

            // create root, left, and right nodes
            Node left = Node();
            Node right = Node();
            
            // add nodes to the tree
            tree_.create_nodes(&parent_node, &left, &right, l_rec, r_rec);

            //    push L and R to stack
            stack_.push(left);
            stack_.push(right);

            tree_.add_node(left);
            tree_.add_node(right);

            node_index += 2;
        } else {
            //    set as leaf
            // Node leaf_node = Node();
            // curr_mode = 'leaf';
            //tree_.create_node(leaf_node, curr_node, curr_mode);
            // set node to leaf
            tree_.add_node(curr_node);
            // make the node a leaf, it shouldn't be split

        }
    }
    //    end while
}