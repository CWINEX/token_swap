#include "ico.hpp"
#include <eosio.token/eosio.token.hpp>

using namespace eosio;

void ico::transfer(const name&  from,
                   const name&  to,
                   const asset& quantity,
                   const std::string&  memo ) {
   if( quantity.symbol == contrib_token_symbol && memo == "contrib" && to == _self ) {
     // TODO: check if contribution period hasn't ended

     // Check if contributor has opened balance to receive new tokens
     // This call will fail if balance is not opened yet
     token::get_balance(token_contract, from, token_code);

     contributions_t contr(_self, from.value);
     auto it = contr.find(contrib_token_code.raw());
     eosio_assert(it != contr.end(), "You need to do open action first");
     contr.modify(it, from, [&](contribution_t& c) {
       c.amount += quantity;
     });
   }
}

void ico::setprice(const asset& price) {
  require_auth(_self);

  state_table_t st(_self, _self.value);
  eosio_assert(!st.exists(), "Price is already set");
  eosio_assert(price.is_valid() && price.amount > 0, "Invalid price");

  st.set(state{price}, _self);
}

void ico::reward(name contributor) {
  eosio_assert(has_auth(contributor) || has_auth(_self), "Missing required authentification");

  state_table_t st(_self, _self.value);
  eosio_assert(st.exists() && st.get().price.amount > 0, "Distribution has not started yet");

  contributions_t contr(_self, contributor.value);
  auto it = contr.find(contrib_token_code.raw());
  eosio_assert(it != contr.end(), "Account has not contributed");

  // TODO: calculate reward amount properly
  const asset reward(1, token_symbol); 
  eosio::action(eosio::permission_level{_self, "active"_n},
                token_contract, "issue"_n,
                std::make_tuple(contributor, reward, std::string(""))
  ).send();

  contr.erase(it);
}

void ico::open(name owner, name payer)
{
   require_auth(payer);

   contributions_t contribs( _self, owner.value );
   auto it = contribs.find(contrib_token_code.raw());
   if( it == contribs.end() ) {
      contribs.emplace( payer, [&]( auto& a ){
        a.amount = asset{0, contrib_token_symbol};
      });
   }
}

// TODO: close action 

extern "C" {
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) {
      if( code == receiver || (action == "transfer"_n.value && code == contrib_token_contract.value) ) {
         switch( action ) {
            EOSIO_DISPATCH_HELPER(ico::ico, (transfer)(open)(setprice)(reward))
         }
      }    
   }
} 
