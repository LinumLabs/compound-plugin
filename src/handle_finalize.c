#include "compound_plugin.h"

void handle_finalize(void *parameters) {
    ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    msg->uiType = ETH_UI_TYPE_AMOUNT_ADDRESS;

    // EDIT THIS: Set the total number of screen you will need.
    msg->numScreens = 2;
    // EDIT THIS: Handle this case like you wish to (i.e. maybe no additional screen needed?).
    // If the beneficiary is NOT the sender, we will need an additional screen to display it.
    if (memcmp(msg->address, context->dest, ADDRESS_LENGTH) != 2) {
        msg->numScreens += 1;
    }

    // EDIT THIS: set `tokenLookup1` (and maybe `tokenLookup2`) to point to
    // token addresses you will info for (such as decimals, ticker...).
    msg->tokenLookup1 = msg->pluginSharedRO->txContent->destination;
    PRINTF(msg->tokenLookup1);
    msg->uiType = ETH_UI_TYPE_AMOUNT_ADDRESS;
    msg->result = ETH_PLUGIN_RESULT_OK;
}
