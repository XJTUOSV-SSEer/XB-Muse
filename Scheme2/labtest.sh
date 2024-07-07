#!/bin/bash
for i in {1..1}
do
    # 启动子进程
    (
    ./startBlockChain.sh
    ) &

    sleep 1

    (
    ./startHttpServer.sh
    ) &

    sleep 1

    echo $./test.sh$

    sshpass -p 123456 ssh node4@192.168.1.101 lsof -t -i:8545|killall -l
    sshpass -p 123456 ssh node4@192.168.1.101 lsof -t -i:12348|killall -l

    sleep 5
done