/**
 * @file config-manager.cc
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

#include <iostream>
#include <fstream>

#include "config-manager.h"

namespace vne
{
std::shared_ptr<ConfigManager> ConfigManager::_instance = nullptr;
std::shared_ptr<ConfigManager> ConfigManager::Instance()
{
    if (_instance == nullptr) {
        _instance = std::shared_ptr<ConfigManager>(new ConfigManager);
    }
    return _instance;
}
void ConfigManager::lockConfigs()
{
    lock_configs = true;
}
void ConfigManager::Destroy()
{
    if (_instance == nullptr)
        return;
    _instance.reset();
    _instance = nullptr;
    return;
}
void ConfigManager::loadConfig(const std::string &p)
{
    _instance = std::shared_ptr<ConfigManager>(new ConfigManager(p));
}
ConfigManager::ConfigManager() : lock_configs(false), _pt(toml::parse(vne::config_path))
{
    config_path = vne::config_path;
}
ConfigManager::ConfigManager(const std::string &p) : lock_configs(false), _pt(toml::parse(p))
{
    config_path = p;
}

ConfigManager::~ConfigManager() {}
void ConfigManager::saveConfigFile()
{
    std::ofstream conf;
    conf.open(config_path);
    conf << toml::format(_pt);
    conf.close();
}
}  // namespace vne
