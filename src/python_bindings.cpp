#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "decision_tree.h"
#include "decision_tree_classifier.h"

#include <iostream>
#include <vector>


namespace py = pybind11;

PYBIND11_MODULE(PairtreeOrchard, m) {

  py::class_<Node>(m, "Node")
      .def(py::init<>())
      .def_readwrite("parent_id", &Node::parent_id_)
      .def_readwrite("left_child_id", &Node::left_child_id_)
      .def_readwrite("right_child_id", &Node::right_child_id_)
      .def_readwrite("is_leaf", &Node::is_leaf_)
      .def("print", &Node::printNode);

  py::enum_<MaxFeaturesMethod>(m, "MaxFeaturesMethod")
      .value("sqrt_method", MaxFeaturesMethod::sqrt_method)
      .value("log2_method", MaxFeaturesMethod::log2_method)
      .value("all_method", MaxFeaturesMethod::all_method);

  py::class_<DecisionTree>(m, "DecisionTree")
      .def_readwrite("nodes", &DecisionTree::nodes_)
      .def(py::init<>())  // default constructor
      .def_readwrite("is_built", &DecisionTree::is_built_)
      .def("add_node", &DecisionTree::addNode)
      .def("create_node", &DecisionTree::createNode)
      .def("create_nodes", &DecisionTree::createNodes)
      .def("print_tree", &DecisionTree::printTree);

  py::class_<DecisionTreeClassifier>(m, "DecisionTreeClassifier")
      .def(py::init<>())
      .def(py::init<int, int, int, int, double>(),
            py::arg("max_depth"),
            py::arg("min_samples_split"), 
            py::arg("min_samples_leaf"),
            py::arg("max_features"),
            py::arg("min_impurity_split"))
      .def_readwrite("tree", &DecisionTreeClassifier::tree_)
      .def("print", &DecisionTreeClassifier::print)
      .def("build_tree", (void (DecisionTreeClassifier::*)(
                          const std::vector<std::vector<double>>,
                          const std::vector<int>)) 
                          &DecisionTreeClassifier::buildTree);


    }
