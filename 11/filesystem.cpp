#include <string_view>
#include <filesystem>
#include <iostream>
#include <regex>

void printDirectory(std::string_view path, int ident=0) {
  std::filesystem::directory_iterator dit{path};
  for(auto&& f: dit) {
    std::cout<<std::string(ident, ' ')<<f.path().filename().c_str()<<std::endl; 
    if(f.is_directory()) {
      printDirectory(f.path().string(), ident+1);
    } 
  }
}

void changeExtension(std::filesystem::path path, std::string fileNames, std::string_view newExtension) {
  std::filesystem::directory_iterator dit{path};
  std::regex matcher{fileNames};

  for(auto&& f: dit) {
    if(!f.is_directory()) {
      auto fn = f.path().filename();
      if(std::regex_match(fn.c_str(), matcher)) {
        auto nfn = fn;
        nfn.replace_extension(newExtension);
        std::filesystem::copy(fn, path / nfn);
      }
    } 
  }
}

int main() {
  printDirectory(".");
  changeExtension("test", ".*\\.cpp", "test");
}
