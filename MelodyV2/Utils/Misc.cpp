
#include "Misc.h"
#include "Math.h"
#include "String.h"
#include "Audio.h"
#include <windows.h>
#include <stdlib.h> 
#include <chrono>
#include <playsoundapi.h>
#include <algorithm>
#include <TlHelp32.h>
#include "System/FileSystem.h"
#include "System/Internet.h"

Audio audioManager;


void Misc::PlaySoundFromUrl(const std::string& url, float volume, bool loop) {
    FileSystem::CreateDirectoryP(FileSystem::AssetDirectory);

    //dw about this
    audioManager.BasePath = FileSystem::AssetDirectory;

    // Get the file name from the url
    std::string fileName = url.substr(url.find_last_of('/') + 1);
    std::string filePath = FileSystem::AssetDirectory + "\\" + fileName;

    // Download the file if it doesn't exist
    if (!FileSystem::FileExists(filePath)) {
        //Logger::Write("PlaySound", "Downloading sound from url " + url + " to " + filePath, Logger::LogType::Debug);
        auto file = Internet::DownloadFile(url, filePath);
        if (!file) {
           // Logger::Write("PlaySound", "Failed to download sound with name " + fileName, Logger::LogType::Error);
            return;
        }
    }

    //Logger::Write("PlaySound", "Playing sound with name " + filePath, Logger::LogType::Debug);
    audioManager.Play(fileName, volume, loop);
}