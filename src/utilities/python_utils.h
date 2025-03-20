//
// Created by Rob Ballantyne on 2025-02-06.
//

#ifndef VNE_MCTS_PYTHON_UTILS_H
#define VNE_MCTS_PYTHON_UTILS_H

#ifdef WITH_FNSS_SUPPORT
#include <string>

#include "Python.h"

namespace vne
{
namespace utilities
{

    class PythonRunner
    {
       public:
        PythonRunner();
        void run(const std::string &script);
        int get_status_code();

       private:
        PyConfig config;
        PyStatus status;
        int status_code;
    };
}  // namespace utilities
}  // namespace vne

#endif

#endif  //VNE_MCTS_PYTHON_UTILS_H
