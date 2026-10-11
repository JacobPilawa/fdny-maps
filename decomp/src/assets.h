#pragma once
#include <filesystem>
#include <cstdlib>
inline const std::string& assetDirectory(){
 static std::string resolved=[](){
  namespace fs=std::filesystem;
  auto valid=[](const fs::path& path){return fs::is_regular_file(path/"entities.json")&&fs::is_regular_file(path/"world_mesh.json");};
  if(const char* overridePath=std::getenv("FDNY_ASSETS_DIR")){
   fs::path path=overridePath;if(!valid(path))throw std::runtime_error("FDNY_ASSETS_DIR does not contain Training assets");return fs::absolute(path).string();
  }
  char* base=SDL_GetBasePath();if(!base)throw std::runtime_error(SDL_GetError());fs::path executableDirectory=base;SDL_free(base);
  // SDL on macOS may return the bundle Resources directory or MacOS directory.
  for(auto path:{executableDirectory/"training",executableDirectory/"../Resources/training",executableDirectory/"assets/training",executableDirectory/"../assets/training",fs::path(FDNY_ASSETS)})if(valid(path))return fs::weakly_canonical(path).string();
  throw std::runtime_error("Cannot locate Training assets beside the application");
 }();return resolved;
}
