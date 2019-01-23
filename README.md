# ICO contract 
You need to set the right permissions in order for reward action to work:
```
cleos set account permission <contract-account> active '{"threshold": 1,"keys": [{"key": "<account-public-key>","weight": 1}],"accounts": [{"permission":{"actor":"<contract-account>","permission":"eosio.code"},"weight":1}]}' owner -p <contract-account>
```

