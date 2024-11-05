# Available at setup time due to pyproject.toml
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup
from glob import glob


__version__ = "0.0.1"

# The main interface is through Pybind11Extension.
# * You can add cxx_std=11/14/17, and then build_ext can be removed.
# * You can set include_pybind11=false to add the include directory yourself,
#   say from a submodule.
#
# Note:
#   Sort input source files if you glob sources to ensure bit-for-bit
#   reproducible builds (https://github.com/pybind/python_example/pull/53)

# ext_modules = [
#     Pybind11Extension(
#         "PairtreeOrchard",
#         ["src/python_bindings.cpp"],
#         # Example: passing in the version to the compiled code
#         define_macros=[("VERSION_INFO", __version__)],
#     ),
# ]

ext_modules = [
    Pybind11Extension(
        "PairtreeOrchard",
        sorted(glob("src/*.cpp")),  # Sort source files for reproducibility
    ),
]

setup(..., ext_modules=ext_modules)

setup(
    name="PairtreeOrchard",
    version=__version__,
    author="David L Gibbs",
    author_email="gibbsdavidl@gmail.com",
    url="https://github.com/gibbsdavidl/PairtreeOrchard",
    description="Grow an orchard of pairtrees.",
    long_description="",
    ext_modules=ext_modules,
    extras_require={"test": "pytest"},
    # Currently, build_ext only provides an optional "highest supported C++
    # level" feature, but in the future it may provide more features.
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.8",
)
