#pragma once

#include <iostream>
#include <pulse/pulseaudio.h>
#include <stdexcept>
#include <string>
#include <vector>

#include "card_profile_info.h"

/// @brief My C++ version of pa_card_info
class CardInfo {
private:
    int index;
    std::string card_name;
    std::string driver_name;
    std::vector<CardProfileInfo> profiles;
    uint32_t current_profile;
public:
    CardInfo(const pa_card_info *card_info);
    int get_index() const;
    std::string get_card_name() const;
    std::vector<CardProfileInfo> get_profiles();
    CardProfileInfo *get_current_profile();
    std::string get_profile_name() const;

    static std::vector<CardInfo> GET_CARD_INFO_LIST();
};