# VNELIB

This directory contains the source and python project files
required to build a wheel file (python package format) that
needs to be installed into your python (virtual) environment.

Recommended use:

1. Setup a Python virtual environment in a convienent location
( python -m venv vnesim-python )
2. Activate and use the environment (source ./vnesim-python/bin/activate, on Linux/MacOS )
3. Install required packages (pip install -r scripts/requirements.txt)
4. Download fnss package somewhere else (git clone https://github.com/fnss/fnss.git)
5. cd into cloned fnss and install (cd FNSS_LOC; git setup.py install)
6. cd to this package and build (cd VNESIM_ROOT/python-package; python -m build)
5. Install the vnelib.whl file (pip install dist/vnelib-....whl)