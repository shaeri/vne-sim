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

#include "config-manager.h"

namespace vne {
    std::shared_ptr<ConfigManager> ConfigManager::_instance = nullptr;
    std::shared_ptr<ConfigManager> ConfigManager::Instance ()
    {
        if (_instance==nullptr) {
            _instance = std::shared_ptr<ConfigManager> (new ConfigManager);
        }
        return _instance;
    }
    void
    ConfigManager::Destroy()
    {
    	if (_instance==nullptr)
    		return;
    	_instance.reset();
    	_instance = nullptr;
    	return;
    }
    ConfigManager::ConfigManager () :
    		lock_configs (false)
    {
    	read_xml (Config_File_Path, _pt);
    }
    ConfigManager::~ConfigManager()
    {
    }
    void
    ConfigManager::saveConfigFile()
    {
    	write_xml (Config_File_Path, _pt);
    }
}
