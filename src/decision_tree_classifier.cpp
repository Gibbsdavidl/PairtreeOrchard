

// DT Classifier definitions

// default DT classifier
DecisionTreeClassifier::DecisionTreeClassifier() {
    max_depth_ = 6;
    min_samples_split_ = 2;
    min_samples_leaf_ = 1;
    max_features_ = 2;
    min_impurity_split_ = 0.0;
}


DecisionTreeClassifier::DecisionTreeClassifier(
  int max_depth,
  int min_samples_split,
  int min_samples_leaf, 
  int max_features,
  double min_impurity_split ) {
    max_depth_ = max_depth;
    min_samples_split_ = min_samples_split;
    min_samples_leaf_ = min_samples_leaf;
    max_features_ = max_features;
    min_impurity_split_ = min_impurity_split;  
}


void DecisionTreeClassifier::print() { 
  std::cout << "DT Classifier" << std::endl;
  std::cout << "Max Depth: " << max_depth_ << std::endl;
  std::cout << "D Tree" << std::endl;
  tree_.print_tree(); 
}


void DecisionTreeClassifier::load_data(
  const std::vector<int> label_data,
  const std::vector<std::vector<double>> feature_data) {
    label_data_ = label_data;
    feature_data_ = feature_data;
    std::cout << "label length: " << label_data_.size() << std::endl;
} 


void DecisionTreeClassifier::add_node() {

	  // Define a distribution
    std::uniform_int_distribution<int> distribution(1, 100);

    // Define a random node
	  Node current_node;
	  current_node.parent_id = distribution(generator);
    //current_node.impurity = current_record.impurity;
    current_node.is_leaf = 0;
    current_node.is_left = 0;
    current_node.n_samples = distribution(generator);
    //current_node.split_feature = current_split.feature;
    current_node.variable1 = distribution(generator);
    current_node.variable2 = distribution(generator);

    tree_.add_node(current_node);
}

  void BuildTree(const std::vector<std::vector<double>> *feature_data_ptr,
                 const std::vector<int> *label_data_ptr,
                 const std::vector<int> samples_subset, 
                 int n_labels,
                 std::mt19937 *gen_);

  void BuildTree(const std::vector<std::vector<double>> feature_data,
                 const std::vector<int> label_data);