// idea is to parse a json file
// settings are loaded before every frame starts

// TEMPORARY NOT IMPLEMENTED parsing json settings are only done when i press apply on a gui

#include "core/FrameLimit.h"
#include "core/simdjson.h"
#include <string>

class Settings {
private:
    std::string c_currentSettingItem {};
    // store the settings here
    std::string c_jsonSettings {}; 
public:

    // load whatevers stored in c_jsonSettings
    Settings(FrameLimit& p_framelimitObj);
    ~Settings();
    void LoadSettings(FrameLimit& p_framelimitObj);    
};
