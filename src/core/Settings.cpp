#include "core/Settings.h"
#include "core/simdjson.h"
#include <cerrno>
#include <fstream>

Settings::Settings(FrameLimit &p_framelimitObj) {
  this->LoadSettings(p_framelimitObj);
}
Settings::~Settings() {}

void Settings::LoadSettings(FrameLimit &p_framelimitObj) {

  /*
  {
  // parsing
  simdjson::ondemand::parser parser;
  auto json = simdjson::padded_string::load("../../resource/settings.json");
  simdjson::ondemand::document doc = parser.iterate(json);

  // loading the settings
  // ++++=========================== goback here
  }
  */

  // INFORMAL settings
  // remove later

  {
    //   std::ifstream in(filename, std::ios::binary);
    //   if (!in){
    //       std::cerr << "ERROR in get_file_contents: unable to open file " <<
    //       filename << std::endl; throw(errno);
    //   }
    //   else{
    //       std::string contents;
    //       in.seekg(0, std::ios::end);
    //       contents.resize(in.tellg());
    //       in.seekg(0,std::ios::beg);
    //       in.read(&contents[0], contents.size());
    //       in.close();
    // /*
    //  *std::streampos size = in.tellg();
    //       if (size > 0) {
    //           contents.resize(static_cast<size_t>(size));
    //           in.seekg(0, std::ios::beg);
    //           in.read(&contents[0], contents.size());
    //
    // */
    //       return contents;
    //   }
  }

  // parsing
  std::ifstream fileSettings("resource/settings.json");
  if (!fileSettings.is_open()) {
    std::cerr << "dafak cannot open this shit" << std::endl;
    // throw(errno);
    return; // or just return early to skip the frame and dont throw shit
  }

  std::string inputStream;
  while (std::getline(fileSettings, inputStream, ':')) {
    // setting the settings
    // std::cout << "watadak" << inputStream << std::endl;

    // the actual value is set after another loop
    if (c_currentSettingItem == "framerate") {
      // only works when no thrown error was made
      p_framelimitObj.SetTargetFPS(std::stof(inputStream)); // string to float

      // use this to catch the error thrown in parsing
      // try {
      //     p_framelimitObj.SetTargetFPS(std::stof(inputStream));
      // } catch (const std::invalid_argument&) {
      //     std::cerr << "Invalid value for setting '" << c_currentSettingItem
      //     << "': " << inputStream << std::endl;
      // }
    }
    c_currentSettingItem = inputStream;
  }
  fileSettings.close();
}
