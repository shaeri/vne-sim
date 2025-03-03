//
// Created by Rob Ballantyne on 2025-02-06.
//

#include "python_utils.h"
#include "core/config-manager.h"
#include "utilities/logger.h"
#include <boost/algorithm/string.hpp>

using namespace std;

namespace vne {
    namespace utilities {

        PythonRunner::PythonRunner() {
            string pythonPath = ConfigManager::Instance()->getConfig<string>("utilities.pythonPath");
            boost::trim(pythonPath);
            PyConfig_InitPythonConfig(&config);
            status = PyConfig_SetBytesString(&config,  &config.program_name, (pythonPath != "" ? pythonPath.c_str() : NULL));
            if (PyStatus_Exception(status)) {
                Logger::Instance()->logFatal("Cannot initialize the python environment to run FNSS for network generation.");
                exit(-1);
            }

            status = Py_InitializeFromConfig(&config);
            if (PyStatus_Exception(status)) {
                Logger::Instance()->logFatal("Cannot initialize the python environment to run FNSS for network generation.");
                exit(-1);
            }
            PyConfig_Clear(&config);
        }

        void PythonRunner::run(const std::string& script) {
            PyRun_SimpleString(script.c_str());
            status_code = Py_FinalizeEx();
        }

        int PythonRunner::get_status_code() {
            return status_code;
        }
    }
}
