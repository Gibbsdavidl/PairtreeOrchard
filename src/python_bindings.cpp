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
      .def_readwrite("value", &Node::value)
      .def("Print", &Node::print_node);

  py::enum_<MaxFeaturesMethod>(m, "MaxFeaturesMethod")
      .value("sqrt_method", MaxFeaturesMethod::sqrt_method)
      .value("log2_method", MaxFeaturesMethod::log2_method)
      .value("all_method", MaxFeaturesMethod::all_method);

  py::class_<DecisionTree>(m, "DecisionTree")
      .def(py::init<>())
      .def("print_tree", &DecisionTree::print_tree)
      .def("add_node", &DecisionTree::add_node)
      .def_readwrite("nodes", &DecisionTree::nodes)
      .def_readwrite("is_built", &DecisionTree::is_built);

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
      .def("load_data", (void (DecisionTreeClassifier::*)(
                       const std::vector<int>,
                       const std::vector<std::vector<double>>
                       )) &DecisionTreeClassifier::load_data)
      .def("add_node", &DecisionTreeClassifier::add_node);


    }
