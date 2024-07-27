#pragma once
#include <iostream>
#include <random>

class Misc {
public:
    static void SetClipboard(std::string str);

    static void PlaySoundFromUrl(const std::string& url, float volume = 1.f, bool loop = false);

};