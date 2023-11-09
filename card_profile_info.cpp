#include "card_profile_info.h"

CardProfileInfo::CardProfileInfo(pa_card_profile_info2 *card_profile) {
    name = std::string(card_profile->name);
    description = std::string(card_profile->description);
    available = card_profile->available;
}

std::string CardProfileInfo::get_name() const {
    return name;
}

std::string CardProfileInfo::get_description() const {
    return CardProfileInfo::description;
}

int CardProfileInfo::get_available() const {
    return available;
}