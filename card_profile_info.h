#pragma once

#include <string>
#include <pulse/pulseaudio.h>

/// @brief My C++ version of pa_card_profile_info
class CardProfileInfo {
private:
    std::string name;
    std::string description;
    int available;
public:
    CardProfileInfo(pa_card_profile_info2 *card_profile);
    std::string get_name() const;
    std::string get_description() const;
    int get_available() const;
};
