#include "card_info.h"

/// @brief Enum for tracting Pulse Server connection state
enum connection_state {
    CONNECTION_READY = 1,
    CONNECTION_FAILED = 2
};

/// @brief Enum for tracking Pulse Server request state
enum operation_state {
    OPERATION_NONE = 0,
    OPERATION_WAIT = 1,
    OPERATION_COMPLETE = 2
};

/// @brief Callback to determine the ready state of pulse server connection
/// @param ctx Pulse Server Context
/// @param data User data to track state
void pa_state_callback(pa_context *ctx, void *data) {
    int *state = (int*)data;
    switch (pa_context_get_state(ctx)){
        case PA_CONTEXT_READY:
            *state = CONNECTION_READY;
            break;
        case PA_CONTEXT_FAILED:
        case PA_CONTEXT_TERMINATED:
            *state = CONNECTION_FAILED;
            break;
        case PA_CONTEXT_UNCONNECTED:
        case PA_CONTEXT_CONNECTING:
        case PA_CONTEXT_AUTHORIZING:
        case PA_CONTEXT_SETTING_NAME:
        default:
            break;
    }
}

/// @brief Callback that runs each time the pulse server is ready to tell us about each card
/// @param ctx Connection context
/// @param card_info Card information
/// @param eol End of list flag
/// @param data User data
void pa_card_info_callback(pa_context *ctx, const pa_card_info *card_info, int eol, void *data) {
    auto card_info_list = (std::vector<CardInfo> *)data;
    // if eol is positive, you're at the end of the list
    if (eol > 0) {
        return;
    }
    card_info_list->push_back(CardInfo(card_info));
}


CardInfo::CardInfo(const pa_card_info *card_info) {
    index = card_info->index;
    card_name = std::string(card_info->name);
    for (uint32_t i = 0; i < card_info->n_profiles; i++) {
        profiles.push_back(CardProfileInfo(card_info->profiles2[i]));
        if (card_info->active_profile2 == card_info->profiles2[i]) {
            current_profile = i;
        }
    }
}

int CardInfo::get_index() const {
    return index;
}

std::string CardInfo::get_card_name() const {
    return card_name;
}

std::vector<CardProfileInfo> CardInfo::get_profiles() {
    return profiles;
}

CardProfileInfo *CardInfo::get_current_profile() {
    return &profiles.at(current_profile);
}

std::string CardInfo::get_profile_name() const {
    return profiles.at(current_profile).get_name();
}

std::vector<CardInfo> CardInfo::GET_CARD_INFO_LIST() {
    std::vector<CardInfo> card_info_list;
    pa_mainloop *pa_ml = pa_mainloop_new();
    pa_mainloop_api *pa_mlapi = pa_mainloop_get_api(pa_ml);
    pa_context *pa_ctx = pa_context_new(pa_mlapi, "test");
    pa_operation *pa_op = nullptr;
    int conn_state = 0;
    int op_state = 0;
    // connect to the pulse server
    pa_context_connect(pa_ctx, nullptr, PA_CONTEXT_NOFLAGS, nullptr);
    pa_context_set_state_callback(pa_ctx, pa_state_callback, &conn_state);
    for (;;) {
        switch (conn_state) {
            case 0:
                if (pa_mainloop_iterate(pa_ml, 1, nullptr) < 0) {
                    throw std::runtime_error("Exit requested");
                }
                continue;
            case 2:
                pa_context_disconnect(pa_ctx);
                pa_context_unref(pa_ctx);
                pa_mainloop_free(pa_ml);
            default:
                break;
        }
        switch (op_state) {
            case OPERATION_NONE:
                pa_op = pa_context_get_card_info_list(pa_ctx, pa_card_info_callback, &card_info_list);
                op_state = OPERATION_WAIT;
            case OPERATION_WAIT:
                if (pa_operation_get_state(pa_op) == PA_OPERATION_DONE) {
                    pa_operation_unref(pa_op);
                    pa_context_disconnect(pa_ctx);
                    pa_context_unref(pa_ctx);
                    pa_mainloop_free(pa_ml);
                    op_state = OPERATION_COMPLETE;
                    return card_info_list;
                }
                break;
            default:
                throw std::runtime_error("Invalid operation state!");
        }
        if (pa_mainloop_iterate(pa_ml, 1, nullptr) < 0) {
            throw std::runtime_error("Exit requested");
        }
    }
    throw std::runtime_error("Error");
}