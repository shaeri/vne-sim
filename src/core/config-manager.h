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
#include <experimental/tuple>

#include "config.h"
#include "toml.hpp"

namespace vne {
    typedef toml::basic_value<toml::type_config>  ConfigType;
    class ConfigManager {
    public:
        static std::shared_ptr<ConfigManager> Instance();
        static void Destroy();
        void saveConfigFile ();
        template<typename T, typename... ARGS>
        const T getConfig (const ARGS&... conf) const;
        template<typename T, typename... ARGS>
        bool setConfig (T& val, const ARGS&... conf);
        ~ConfigManager ();
    protected:
         ConfigManager ();
    private:
        mutable bool lock_configs;
        static std::shared_ptr<ConfigManager> _instance;
        toml::basic_value<toml::type_config> _pt;
    };


    // Base case: Set the value for the final key
    template <typename T, typename Key>
    inline void set_nested_value_impl(T& new_value, toml::value& current, const Key& key) {
        if (!current.is_table()) {
            throw std::runtime_error("Current node is not a table");
        }
        current.as_table()[key] = toml::value(new_value);
    }

    // Recursive case: Traverse nested tables using keys
    template <typename T, typename FirstKey, typename SecondKey, typename... RestKeys>
    inline void set_nested_value_impl(T& new_value, toml::value& current, const FirstKey& first_key, const SecondKey& second_key, const RestKeys&... rest_keys) {
        if (!current.is_table()) {
            throw std::runtime_error("Current node is not a table");
        }

        auto& table = current.as_table();
        // const auto key = std::forward<FirstKey>(first_key);
        auto it = table.find(first_key);

        if (it == table.end()) {
            throw std::runtime_error("key you want to set is not in config.toml");
        }

        // Recurse with the next key and remaining keys
        set_nested_value_impl(new_value, it->second, second_key, rest_keys...);
    }

    // Entry point for variadic template
    template <typename T, typename... Keys>
    inline void set_nested_value(T& new_value, toml::value& root, const Keys&... keys) {
        set_nested_value_impl(new_value, root, keys...);
    }
    template<typename T, typename... ARGS>
    const T ConfigManager::getConfig (const ARGS&... conf) const
    {
        //if its the first time that a config being read write and reload the config file.
        if (!lock_configs)
            lock_configs = true;

        return toml::find<T>(_pt, conf...);
    }
    template<typename T, typename... ARGS>
    bool ConfigManager::setConfig (T& val, const ARGS&... conf)
    {
        if (lock_configs)
        {
            std::cerr << "Error in ConfigManager!!!" << std::endl;
            std::cerr << "The configuration file has already been loaded! "
                << "\n You can not write on the configuration file anymore."
                << " setConfig can only be called before any calls to getConfig." << std::endl;
            return false;
        }

        try {
            set_nested_value<T, ARGS...>(val, _pt, conf...);
            return true;
        }
        catch (...) {
            std::cerr << "The configuration that you want to modify does not exist." << std::endl;
            return false;
        }
    }
}

#endif
