﻿/**
 *  @dev minakokojima, yukiexe
 */

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <string>

#define EOS S(4, EOS)
#define TOKEN_CONTRACT N(eosio.token)

using namespace eosio;
using namespace std;

struct st_transfer {
    eosio::name from;
    eosio::name to;
    asset       quantity;
    string      memo;

    EOSLIB_SERIALIZE( st_transfer, (from)(to)(quantity)(memo) )
};

CONTRACT escrow : public eosio::contract {
public:
    using contract::contract;

    void onTransfer(eosio::name from, eosio::name to, extended_asset quantity, std::string memo) {        
    
        if (to != _self) return;
    
        require_auth(from);

        eosio_assert(quantity.quantity.is_valid(), "invalid token transfer");
        eosio_assert(quantity.quantity.amount > 0, "must transfer a positive amount");

        /*    
        auto a = asset(quantity.quantity.symbol, quantity.quantity.amount / 2);
        auto b = asset(quantity.quantity.symbol, quantity.quantity.amount - quantity.quantity.amount / 2);

        if (a.amount > 0) {
            action(
                permission_level{_self, "active"_n},
                quantity.contract, N(transfer),
                make_tuple(_self, N(minakokojima), a,
                std::string(""))
            ).send();
        }
        if (b.amount > 0) {
            action(
                permission_level{_self, "active"_n},
                quantity.contract, "transfer"_n),
                make_tuple(_self, "rukamoemoe51"_n), b,
                std::string(""))
            ).send();
        }*/
    } // onTransfer()

    void apply(uint64_t code, uint64_t action) {
        auto &thiscontract = *this;
        if (action == eosio::name("transfer").value) {
            auto transfer_data = unpack_action_data<st_transfer>();
            onTransfer(transfer_data.from, transfer_data.to, extended_asset(transfer_data.quantity, eosio::name(code)), transfer_data.memo);
            return;
        }
    }
};


extern "C" {
    [[noreturn]] void apply(uint64_t receiver, uint64_t code, uint64_t action) {
        escrow p( name(receiver), name(code), datastream<const char*>(nullptr, 0) );
        p.apply(code, action);
        eosio_exit(0);
    }
}