#include "compound_plugin.h"

#define NUM_COMPOUND_BINDINGS 9

const compoundAssetDefinition_t UNDERLYING_ASSET_DECIMALS[NUM_COMPOUND_BINDINGS] = {
    {"cDAI", 18},
    {"CETH", 18},
    {"CUSDC", 6},
    {"CZRX", 18},
    {"CUSDT", 6},
    {"CBTC", 8},
    {"CBAT", 18},
    {"CREP", 18},
    {"cSAI", 18},
};

bool get_underlying_asset_decimals(char *compound_ticker, uint8_t *out_decimals) {
    for (size_t i = 0; i < NUM_COMPOUND_BINDINGS; i++) {
        compoundAssetDefinition_t *binding =
            (compoundAssetDefinition_t *) PIC(&UNDERLYING_ASSET_DECIMALS[i]);
        if (strncmp(binding->ticker, compound_ticker, strnlen(binding->ticker, MAX_TICKER_LEN)) ==
            0) {
            *out_decimals = binding->decimals;
            return true;
        }
    }
    return false;
}

void handle_provide_token(void *parameters) {
    ethPluginProvideInfo_t *msg = (ethPluginProvideInfo_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;
    msg->result = get_underlying_asset_decimals("cDAI", &context->decimals)
                          ? ETH_PLUGIN_RESULT_OK
                          : ETH_PLUGIN_RESULT_FALLBACK;
    if (msg->item1) {
        msg->result = get_underlying_asset_decimals(msg->item1->token.ticker, &context->decimals)
                          ? ETH_PLUGIN_RESULT_OK
                          : ETH_PLUGIN_RESULT_FALLBACK;
        strlcpy(context->ticker, (char *) msg->item1->token.ticker, sizeof(context->ticker));
        context->token_found = true;
    } else {
        // The Ethereum App did not manage to find the info for the requested token.
        context->token_found = false;

        // Default to ETH's decimals (for wei).
        context->decimals = 18;
        msg->result = ETH_PLUGIN_RESULT_OK;
        // If data wasn't found, use "???" as the ticker.
        msg->additionalScreens++;

        strlcpy(context->ticker,
                "Unknown token. Please contact Ledger support.",
                sizeof(context->ticker));
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}
