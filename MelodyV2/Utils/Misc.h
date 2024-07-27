#pragma once
#include <iostream>
#include <random>

class Misc {
public:
    static void SetClipboard(std::string str);

    static void PlaySoundFromUrl(const std::string& url, float volume = 1.f, bool loop = false);
    static void DownLoadFromUrl(const std::string& url);
    //Vec2<float> GetRotations(Vec3<float> playerEyePos, Vec3<float> targetPos);
};