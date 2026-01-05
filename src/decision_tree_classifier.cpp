#include "decision_tree_classifier.h"
#include "splitter.h"
#include <iostream>
#include <vector>
#include <spdlog/fmt/bundled/ranges.h>


// DT Classifier definitions //


// this is the index covering the samples
std::vector<int> createVectorFromIToJ(int i, int j) {
    std::vector<int> vec;
    for (int k = i; k <= (j-1); ++k) {
        vec.push_back(k);
    }
    return vec;
}

DecisionTreeClassifier::DecisionTreeClassifier() //: 
// criterion_(feature_data_, label_data_),  
// splitter_(feature_data_, label_data_, criterion_)  
{
    // Retrieve the logger created during module import
  logger_ = spdlog::get("pto");
  logger_->info("DecisionTreeClassifier instance created.");

  max_features_ = 100;  // 
  min_samples_leaf_ = 3;      // when to stop splitting due to few samples
  max_depth_ = 8;             // maximum depth of the tree, when to stop
  min_samples_split_ = 8;     // number of samples needed to split?
  min_label_entropy_ = 0.5; // 
}


DecisionTreeClassifier::DecisionTreeClassifier(
    int max_depth,
    int min_samples_split,
    int min_samples_leaf,
    int max_features,
    double min_label_entropy_
  ) //: 
  //   criterion_(feature_data_, label_data_),  
  //   splitter_(feature_data_, label_data_, criterion_)  
  {
    // Retrieve the logger created during module import
    logger_ = spdlog::get("pto");
    logger_->info("DecisionTreeClassifier instance created.");

    max_depth_ = max_depth;
    min_samples_split_ = min_samples_split;
    min_samples_leaf_ = min_samples_leaf;
    max_features_ = max_features;
    min_label_entropy_ = min_label_entropy_;  

  }


void DecisionTreeClassifier::print() { 
  std::cout << "\nDT Classifier" << std::endl;
  std::cout << "Max Depth: " << max_depth_ << std::endl;
  std::cout << "Decision Tree:" << std::endl;
  tree_.printTree(); 
}

Record DecisionTreeClassifier::initRecord(int n_samples)
{
  // start with root node
    Record curr_record = Record();
    curr_record.n_samples_=n_samples;    // number of samples in the set
    curr_record.index_=createVectorFromIToJ(0, n_samples);   // index in to data and labels
    curr_record.entropy_=0.0;       // entropy of this node, updated at search_split
    curr_record.variable1_=-1;        // index into feature rows
    curr_record.variable2_=-1;        // used for pair comparison v1 > v2
    curr_record.prob_v1gtv2_=0.0;  // of v1 > v2 / n_samples
    return(curr_record);
}


void DecisionTreeClassifier::buildTree(
              const std::vector<std::vector<double>> feature_data,
              const std::vector<int> label_data) {


    logger_->debug("Starting to build the tree with {} samples.", feature_data.size());

    // set the data
    label_data_ = label_data;
    feature_data_ = feature_data;

    // extract information about the data
    n_samples_ = label_data_.size();
    if (!feature_data.empty()) {
      n_features_ = feature_data[0].size();
    }

    // initialize the building objects with references to data
    Criterion criterion(feature_data_, label_data_);

    // assert
    // length of one row of feature_data must equal n_samples_

    // assert
    // labels must be 0s and 1s

    // check
    // that labels have both 0s and 1s

    std::cout << "label length: " << n_samples_ << std::endl;
    std::cout << "n_features_ : " << n_features_ << std::endl;
    std::cout << "features cols: " << feature_data_[0].size() << std::endl;

    // create root node, contains the root record
    Record curr_record = initRecord(n_samples_);
    Node parent_node = Node();
    std::string curr_mode = "root";
    int node_index = 0;

    // start growing the tree
    tree_.createNode(parent_node, curr_record, curr_mode, node_index); 
    tree_.addNode(parent_node);

    //  push current to stack
    stack_.push(parent_node);

    int node_counter = 0;
    int max_nodes = 10000; // sanity limit

    //  while there's still something on the stack
    while (!stack_.isEmpty() && node_counter < max_nodes) {
  
        node_counter++;

        std::cout << "stack size: " << stack_.stack_.size() << std::endl;

        //    pop to current
        Node curr_node = Node();
        stack_.pop(curr_node);

        std::cout << "popped with n_samples: " << curr_node.record_.n_samples_ << std::endl;

        // Add this log line!
        logger_->debug("Processing Node ID: {}. Depth: {}. Samples: {}. Entropy: {}",
             curr_node.self_id_,
             curr_node.depth_,
             curr_node.record_.n_samples_,
             curr_node.record_.entropy_); // Assuming entropy is calculated

        Splitter splitter(feature_data_, label_data_, criterion, curr_node.record_.index_);
        splitter.setParams(max_depth_, 
                            min_samples_split_,
                            min_samples_leaf_,
                            max_features_,
                            min_label_entropy_);

        //    if current criteria says to split
        if (splitter.searchSplit(&curr_node, "c45") == true) {
            // best split
            std::cout << "best split found: " << std::endl;
            std::cout << "record.variable1_: " <<curr_node.record_.variable1_ << std::endl;
            std::cout << "record. threshold: " << curr_node.record_.threshold << std::endl;
            std::cout << "record.gain_ratio: " << curr_node.record_.gain_ratio << std::endl;
            logger_->debug("Parent node {} split.", curr_node.self_id_);

            //    split node
            Record l_rec;
            Record r_rec;   // fills in the records
            splitter.split(&curr_node, &l_rec, &r_rec, "c45");

            // Use a helper to print the vector contents
            logger_->debug("Left child gets indices: {}", fmt::join(l_rec.index_, ", "));
            logger_->debug("Right child gets indices: {}", fmt::join(r_rec.index_, ", "));

            // create root, left, and right nodes
            Node left = Node();
            Node right = Node();
            
            // add nodes to the tree
            tree_.createNodes(curr_node, left, right, l_rec, r_rec);

            //    push L and R to stack
            stack_.push(left);
            stack_.push(right);

            node_index += 2;
        } else {
            //    set as leaf
            // Node leaf_node = Node();
            // curr_mode = 'leaf';
            //tree_.create_node(leaf_node, curr_node, curr_mode);
            // set node to leaf, it shouldn't be split
            // Note: leaf nodes are already added to the tree in createNodes
        }
    }
    //    end while
}