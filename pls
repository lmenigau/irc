<< CAP LS
<< PASS 1
<< NICK odessein
<< USER odessein odessein 1 :Orson DESSEIN
>> CAP * LS :multi-prefix
<< CAP REQ :multi-prefix
>> odessein!ip6-localhost NICK odessein
>> :ircserv.localhost 001 odessein :Welcome to the FT_IRC server odessein!odessein@ip6-localhost
<< MODE odessein +i
>> :ircserv.localhost 002 odessein Your host is FT_IRC running version 0.0.1dev
>> :ircserv.localhost 003 odessein This server was created idk like now ?
>> :ircserv.localhost 004 odessein FT_IRC 0.0.1dev ia i
>> CAP * LS :multi-prefix
<< CAP END
>> :ircserv.localhost 221 odessein +i
>> CAP * LS :multi-prefix
<< PING ircserv.localhost
>> :ircserv.localhost PONG ircserv.localhost
--> lag pong
<< NICK b
>> :odessein!~odessein@ip6-localhost NICK b
<< JOIN #m
>> :ircserv.localhost 331 b #m :No topic is set
>> :b!odessein@ip6-localhost JOIN #m
>> :ircserv.localhost 353 b = #m :@b 
>> :ircserv.localhost 366 b #m :End of NAMES list
<< MODE #m
>> :ircserv.localhost 324 b #m 
--> chanquery mode
<< WHO #m
<< MODE #m +i
<< PING ircserv.localhost
>> :ircserv.localhost PONG ircserv.localhost
--> lag pong
<< INVITE a #m
>> :ircserv.localhost 341 ba#m:a!odessein@ip6-localhost JOIN #m
<< MODE #m -t
<< PING ircserv.localhost
>> :ircserv.localhost PONG ircserv.localhost
--> FAILED who: chanquery abort
<< WHO #m
--> lag pong
<< PRIVMSG #m :test
>> :a!odessein@ip6-localhost PRIVMSG #m :mahga
<< PING ircserv.localhost
>> :ircserv.localhost PONG ircserv.localhost
--> lag pong
<< PING ircserv.localhost
>> :ircserv.localhost PONG ircserv.localhost
--> FAILED who: chanquery abort
<< MODE #m b
--> lag pong
>> :ircserv.locahost 367 b #m:irvserv.localhost 368 :End of channel ban list
<< TOPIC #m :test
>> :b TOPIC #m :test
