# Runs the server / client program on FreeBSD without CAP_ENTER enabled
#!/bin/bash
./serverb &
COUNTER=0
while [ $COUNTER -lt 10 ];
do
sleep 1

./clientb read_from.txt write_to.txt

let COUNTER=COUNTER+1
done
