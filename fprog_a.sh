# Execute the program with CAP_ENTER enabled
#!/bin/bash
./server &
COUNTER=0
while [ $COUNTER -lt 10 ];
do
sleep 1
./client read_from.txt write_to.txt

let COUNTER=COUNTER+1
done
