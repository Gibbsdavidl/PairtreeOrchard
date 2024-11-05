# PairtreeOrchard

## Grow a pairtree orchard and harvest your predictions!


Built with generous help from:
https://github.com/pybind/python_example
https://github.com/pybind


compile and install with:
python3 -m pip install ./PairtreeOrchard/

or

## This worked ##

c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) src/decision_tree.cpp src/python_bindings.cpp -o PairtreeOrchard$(python3-config --extension-suffix)

Then from inside the folder...

python3
import PairtreeOrchard as po
po.DecisionTreeClassifier()