#pragma once

#include "wallet/wallet.hpp"
#include "bank/bank.hpp"
#include "merchant/merchant.hpp"

namespace ui {

void run_dashboard(Wallet& wallet, Bank& bank, Merchant& merchant);

}