# discord-api
the starts of a discord api written in openssl-wrapped sockets probably

As of Tue, 6 Apr my development goals are:

- Establish heartbeat loop with Discord's gateway
- ~~Implement JSON encoding/decoding library~~ (finished Thu, 8 Apr)
- Identify with Discord's gateway to authenticate
- Implement basic API manipulation methods

# remarks

As a memo to any onlookers, this project is my ongoing attempt to collate everything I've learned in the past years, I don't ensure (a) ease of use (b) standards
compliance (although I am trying to maintain RFC-compliancy) (c) future-proof code or (d) any safe code. Though I don't program carelessly since I try to `free` 
every dynamic object, ensure bounds-checking and try to provide a somewhat informative debugging interface, I'm fully certain when I say that this code is far
from perfect.

Also, I use my own slightly-personalized take on the GNU C programming style. I'm just a shill for GNU, don't mind me--also, for replication purposes, my build
environment uses `icpc`, the Intel C/C++ Compiler (2021.1.2)
