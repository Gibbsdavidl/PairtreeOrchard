#include "decision_tree_classifier.h"
#include "splitter.h"
#include <iostream>
#include <vector>


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
  max_features_ = 100;  // 
  min_samples_leaf_ = 3;      // when to stop splitting due to few samples
  max_depth_ = 8;             // maximum depth of the tree, when to stop
  min_samples_split_ = 8;     // number of samples needed to split?
  min_impurity_split_ = 0.5; // 
}


DecisionTreeClassifier::DecisionTreeClassifier(
    int max_depth,
    int min_samples_split,
    int min_samples_leaf,
    int max_features,
    double min_impurity_split
  ) //: 
  //   criterion_(feature_data_, label_data_),  
  //   splitter_(feature_data_, label_data_, criterion_)  
  {
    max_depth_ = max_depth;
    min_samples_split_ = min_samples_split;
    min_samples_leaf_ = min_samples_leaf;
    max_features_ = max_features;
    min_impurity_split_ = min_impurity_split;  
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

    // set the data
    label_data_ = label_data;
    feature_data_ = feature_data;

    // extract information about the data
    n_samples_ = label_data_.size();
    n_features_ = feature_data.size();

    // initialize the building objects with references to data
    Criterion criterion_ = Criterion(feature_data, label_data);

    // assert
    // length of one row of feature_data must equal n_samples_

    // assert
    // labels must be 0s and 1s

    // check
    // that labels have both 0s and 1s

    std::cout << "label length: " << n_samples_ << std::endl;
    std::cout << "features rows: " << n_features_ << std::endl;
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

        std::cout << "popped, n_samples: " << curr_node.record_.n_samples_ << std::endl;

        // create a splitter object
        Splitter splitter_ = Splitter(feature_data, label_data, 
                                      criterion_, curr_node.record_.index_);
        splitter_.setParams(max_depth_, 
                            min_samples_split_,
                            min_samples_leaf_,
                            max_features_,
                            min_impurity_split_);

        //    if current criteria says to split
        if (splitter_.searchSplit(&curr_node) == true) {
            //    split node
            Record l_rec;
            Record r_rec;   // fills in the records
            splitter_.split(&curr_node, &l_rec, &r_rec);

            // create root, left, and right nodes
            Node left = Node();
            Node right = Node();
            
            // add nodes to the tree
            tree_.createNodes(parent_node, left, right, l_rec, r_rec);

            //    push L and R to stack
            stack_.push(left);
            stack_.push(right);

            tree_.addNode(left);
            tree_.addNode(right);

            node_index += 2;
        } else {
            //    set as leaf
            // Node leaf_node = Node();
            // curr_mode = 'leaf';
            //tree_.create_node(leaf_node, curr_node, curr_mode);
            // set node to leaf
            tree_.addNode(curr_node);
            // make the node a leaf, it shouldn't be split

        }
    }
    //    end while
}