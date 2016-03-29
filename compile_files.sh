# Compile the 4 programs in FreeBSD
#!/bin/bash
cc server.c -o server
cc client.c -o client
cc serverb.c -o serverb
cc clientb.c -o clientb
