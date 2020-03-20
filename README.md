# MDNS
This file is to MDNS query in C for A and PTR type records

compile

gcc -o mdns.out mdns.c

Run
./mdns.out   "FQDN"  "RType"

e.g
./mdns.out   _printer._tcp.local. A 

Remember a dot after local 

# Hamming 
Generate Hamming code

compile

gcc -o hamming.out hamming.c

./hamming.out <binary string>
