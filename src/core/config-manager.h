/**
 * @file config-manager.h
 * @author Soroush Haeri <soroosh.haeri@me.com>
 * @date 7/16/14
 *
 * @copyright Copyright (c) 7/16/14                      SOROUSH HAERI
 *     All Rights Reserved
 *
 *     Permission to use, copy, modify, and distribute this software and its
 *     documentation for any purpose and without fee is hereby granted, provided
 *     that the above copyright notice appear in all copies and that both that
 *     copyright notice and this permission notice appear in supporting
 *     documentation, and that the name of the author not be used in advertising or
 *     publicity pertaining to distribution of the software without specific,
 *     written prior permission.
 *
 *     THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 *     ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS; IN NO EVENT SHALL
 *     AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
 *     DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 *     AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *     OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 **/

#ifndef CONFIG_MANAGER_H_
#define CONFIG_MANAGER_H_

#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "config.h"

namespace vne {
    using boost::property_tree::ptree;
    class ConfigManager {
    public:
        static std::shared_ptr<ConfigManager> Instance();
        static void Destroy();
        void saveConfigFile ();
        template<typename T>
        const T getConfig (const std::string& conf) const;
        template<typename T>
        bool setConfig (const std::string& conf, T &val);
        ~ConfigManager ();
    protected:
         ConfigManager ();
    private:
        mutable bool lock_configs;
        static std::shared_ptr<ConfigManager> _instance;
        ptree _pt;
    };
    template<typename T>
    const T ConfigManager::getConfig (const std::string& conf) const
    {
    	//if its the first time that a config being read write and reload the config file.
    	if (!lock_configs && conf != "utilities.logFile")
    		lock_configs = true;

        return _pt.get<T> (conf);
    }
    template<typename T>
    bool ConfigManager::setConfig (const std::string& conf, T& val)
    {
    	if (lock_configs)
    	{
    		std::cerr << "Error in ConfigManager!!!" << std::endl;
    		std::cerr << "The configuration file has already been loaded! "
    				<< "\n You can nor write on the configuration file anymore."
    				<< " setConfig can only be called before any calls to getConfig." << std::endl;
    		return false;
    	}
    	boost::optional<T> property = _pt.get_optional<T>(conf);
    	if (!property)
    	{
    		std::cerr << "The configuration that you want to modify does not exist." << std::endl;
    		return false;
    	}
    	_pt.put (conf, val);
    	return true;
    }
}

#endif
