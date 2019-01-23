#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/singleton.hpp>

using namespace eosio;

constexpr name token_contract    = "cwex"_n;
constexpr symbol_code token_code = symbol_code("CWEX");
constexpr symbol token_symbol    = symbol(token_code, 3);

constexpr name contrib_token_contract    = "eosio.token"_n;
constexpr symbol_code contrib_token_code = symbol_code("SYS");
constexpr symbol contrib_token_symbol    = symbol(contrib_token_code, 4);

CONTRACT ico : public eosio::contract {
public:
   using contract::contract;

   ACTION open(name owner, name payer);
   ACTION reward(name contributor);
   ACTION setprice(const asset& price);

   // Transfer notification handler
   void transfer( const name&  from,
                  const name&  to,
                  const asset& quantity,
                  const std::string&  memo );

   // Table scope is used as owner field
   TABLE contribution_t {
      asset amount;

      uint64_t primary_key() const {
         return amount.symbol.code().raw();
      }
   };


   TABLE state {
      asset price;

      // state_t() {
      //    price = 0;
      // }

      EOSLIB_SERIALIZE(state, (price))
   };

   typedef multi_index<"contribution"_n, contribution_t> contributions_t;
   typedef eosio::singleton<"state"_n, state> state_table_t;
};
