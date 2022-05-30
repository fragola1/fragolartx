#include "../plugin_sdk/plugin_sdk.hpp"

PLUGIN_NAME("Nunu");
SUPPORTED_CHAMPIONS(champion_id::Nunu);

#include "nunu.h"

PLUGIN_API bool on_sdk_load(plugin_sdk_core* plugin_sdk_good) {
    DECLARE_GLOBALS(plugin_sdk_good);

    switch (myhero -> get_champion()) {
        case champion_id::Nunu:
            nunu::load();
            break;
        default:
            console -> print("Campeão não suportado!", myhero -> get_model_cstr());
            return false;
    }

    return true;
    
}

PLUGIN_API void on_sdk_unload() {
    switch (myhero -> get_champion()) {
        case champion_id::Nunu:
            nunu::unload();
            break;
        default:
            break;
    }
}
