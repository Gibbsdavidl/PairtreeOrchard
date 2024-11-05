#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <iostream>
#include <vector>

#include "decision_tree.h"
//#include "./core/random_forest.h"

namespace py = pybind11;

PYBIND11_MODULE(PairtreeOrchard, m) {
  py::class_<Node>(m, "Node")
      .def_readwrite("parent_id", &Node::parent_id)
      .def_readwrite("left_child_id", &Node::left_child_id)
      .def_readwrite("right_child_id", &Node::right_child_id)
      .def_readwrite("is_leaf", &Node::is_leaf)
      //.def_readwrite("impurity", &Node::impurity)
      .def_readwrite("split_feature", &Node::split_feature)
      .def_readwrite("variable1", &Node::variable1)
      .def_readwrite("variable2", &Node::variable2)
      .def_readwrite("proportion", &Node::proportion)
      .def_readwrite("n_samples", &Node::n_samples)
      .def_readwrite("value", &Node::value);

  py::enum_<MaxFeaturesMethod>(m, "MaxFeaturesMethod")
      .value("sqrt_method", MaxFeaturesMethod::sqrt_method)
      .value("log2_method", MaxFeaturesMethod::log2_method)
      .value("all_method", MaxFeaturesMethod::all_method);

  py::class_<DecisionTree>(m, "DecisionTree")
      .def_readwrite("nodes", &DecisionTree::nodes);

  py::class_<DecisionTreeClassifier>(m, "DecisionTreeClassifier")
      .def(py::init())
      .def_readwrite("tree", &DecisionTreeClassifier::tree_)
      .def("print_tree", &DecisionTreeClassifier::PrintTree)
      .def("add_node", &DecisionTreeClassifier::AddNode);

  // py::class_<DecisionTreeClassifier>(m, "DecisionTreeClassifier")
  //     .def(//py::init<ImpurityMeasure, 
  //       int, int, int, int, 
  //       MaxFeaturesMethod, double>(),
  //          //py::arg("impurity_measure") = gini,
  //          py::arg("max_depth") = std::numeric_limits<int>::max(),
  //          py::arg("min_samples_split") = 2, 
  //          py::arg("min_samples_leaf") = 1,
  //          py::arg("max_features") = -1,
  //          py::arg("max_features_method") = sqrt_method,
  //          py::arg("min_impurity_split") = 0.0)
  //     //.def_readwrite("tree", &DecisionTreeClassifier::tree_)
  //     //.def("build_tree", (void (DecisionTreeClassifier::*)(
  //     //                       const std::vector<std::vector<double>>,
  //     //                       const std::vector<int>)) &
  //     //                       DecisionTreeClassifier::BuildTree)
  //     .def("print_tree", &DecisionTreeClassifier::PrintTree)
  //     .def("add_node", &DecisionTreeClassifier::AddNode);
  //     //.def("predict_classes", &DecisionTreeClassifier::PredictClasses);

    }