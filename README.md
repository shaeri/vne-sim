# VNE-Sim 🕸

![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=flat&logo=cmake&logoColor=white)
![C++](https://img.shields.io/badge/C++11-%2300599C.svg?style=flat&logo=c%2B%2B&logoColor=white)

A discrete event simulator for Virtual Network Embedding (VNE) algorithms,
written in C++11. This implementation has been used in the article: **S. Haeri**
and **Lj. Trajkovic**,
[_"Virtual network embedding via Monte-Carlo tree search,"_](http://www.sfu.ca/~ljilja/papers/t-cybernetics-2016_web.pdf)
IEEE Transactions on Cybernetics, vol. 48, no. 2, pp. 510-521, Feb. 2018.

---

## 🛠️ Building VNE-Sim

### Prerequisites

| Component              | Purpose                                    | Debian/Ubuntu Package    |
| ---------------------- | ------------------------------------------ | ------------------------ |
| Python 3.x             | Build scripts & FNSS installation          | `python3`                |
| CMake (≥3.5)           | Build system                               | `cmake`                  |
| GNU Scientific Library | Random number generation                   | `libgsl-dev`             |
| GLPK                   | Mixed-integer linear programming           | `libglpk-dev`            |
| Boost                  | Filesystem, logging, threading, unit tests | `libboost-all-dev`       |
| MPI (OpenMPI/MPICH)    | Parallelization for MCTS                   | `libopenmpi-dev`         |
| SQLite3                | Simulation results storage                 | `sqlite3 libsqlite3-dev` |

#### Development only Dependencies

| Component    | Purpose                                                        | Debian/Ubuntu Package    |
| ------------ | -------------------------------------------------------------- | ------------------------ |
| pre-commit   | Installs the pre commit hooks to ensure proper code formatting | pip package (pre-commit) |
| clang-format | code formatting                                                | `clang-format`           |

### 🏗️ Build Steps

1. **Clone and Install Dependencies with CMake**

   ```bash
   git clone https://bitbucket.org/shaeri/vne-sim.git
   # if you do not need FNSS for creating datacenter networks leave FNSS_SUPPORT=OFF
   mkdir build && cd build && cmake .. -DWITH_FNSS_SUPPORT=ON
   ```

2. **Install FNSS Python Package**

   ```bash
   # assuming you are still in the build folder
   cd deps/src/fnss
   # install fnss for user or any virtual env you want to use
   python3 setup.py install --user
   # alternatively you can install it system wide
   sudo python3 setup.py install
   ```

3. **Compile VNE-Sim**

   ```bash
   make -j$(nproc)
   ```

4. **Further steps for development**

   ```bash
   # install the pre-commit hooks
   pre-commit install
   ```

---

## 🚀 Running VNE-Sim

### CLI Command Structure

```bash
./build/bin/vnesim --config [CONFIG_FILE] [SUBCOMMAND] [OPTIONS]
```

#### Subcommands

| Command        | Description                 | Required Options              |
| -------------- | --------------------------- | ----------------------------- |
| `simulate`     | Run a simulation            | `-a [ALGORITHM] -v [VNR_DIR]` |
| `substrategen` | Generate substrate networks | None                          |
| `requestgen`   | Generate VNR datasets       | None                          |

#### Supported Algorithms (`-a` flag)

| Algorithm       | Description                            |
| --------------- | -------------------------------------- |
| `mcvne_bfs_mcf` | MC-VNE with BFS node mapping, MCF link |
| `mcvne_mcf_mcf` | MC-VNE with MCF for both stages        |
| `grc_mcf`       | GRC algorithm with MCF link mapping    |
| `vineyard_d`    | D-Vine algorithm from Vineyard         |
| `vineyard_r`    | R-Vine algorithm from Vineyard         |

**Example**:

```bash
./build/bin/vnesim --config config.toml simulate \
  -a grc_mcf \
  -v ./vnr_requests/
```

---

## 📖 Citation and Papers

If you use this implementation in your research, please cite:

**S. Haeri** and **Lj. Trajkovic**,
[_"Virtual network embedding via Monte-Carlo tree search,"_](http://www.sfu.ca/~ljilja/papers/t-cybernetics-2016_web.pdf)
IEEE Transactions on Cybernetics, vol. 48, no. 2, pp. 510-521, Feb. 2018.

📜 **BibTeX**:

```bibtex
@article{haeri2018vne,
title={Virtual network embedding via Monte-Carlo tree search},
author={Haeri, S. and Trajkovic, Lj.},
journal={IEEE Transactions on Cybernetics},
volume={48},
number={2},
pages={510--521},
year={2018},
month={Feb},
doi={10.1109/TCYB.2017.2653244}

}

```

---
