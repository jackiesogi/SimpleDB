#!/bin/bash

# 先編譯 hirebis 的 benchmark
echo "Compiling benchmark..."
gcc -o test/hirebis/benchmark-SET test/hirebis/benchmark-SET.c src/hirebis.c src/datastructure.c src/crud.c src/loadsave.c src/log.c -I./include -I./src -g
gcc -o test/hirebis/benchmark-GET test/hirebis/benchmark-GET.c src/hirebis.c src/datastructure.c src/crud.c src/loadsave.c src/log.c -I./include -I./src -g
gcc -o test/hirebis/benchmark-DEL test/hirebis/benchmark-DEL.c src/hirebis.c src/datastructure.c src/crud.c src/loadsave.c src/log.c -I./include -I./src -g

# 再編譯 hiredis 的 benchmark
gcc -o test/hiredis/benchmark-SET test/hiredis/benchmark-SET.c -g -lhiredis
gcc -o test/hiredis/benchmark-GET test/hiredis/benchmark-GET.c -g -lhiredis
gcc -o test/hiredis/benchmark-DEL test/hiredis/benchmark-DEL.c -g -lhiredis


# 建立 test-log
echo "Creating log file..."
DATE=$(date +'%Y-%m-%d--%H')
MY_LOG_FILE="test/hirebis/test_log/$DATE"
HI_LOG_FILE="test/hiredis/test_log/$DATE"
touch "$MY_LOG_FILE"
touch "$HI_LOG_FILE"

# 執行 benchmark 並把結果寫入 test-log
echo "Run benchmark..."
test/hirebis/benchmark-SET >> "$MY_LOG_FILE-SET.log"
test/hirebis/benchmark-GET >> "$MY_LOG_FILE-GET.log"
test/hirebis/benchmark-DEL >> "$MY_LOG_FILE-DEL.log"

test/hiredis/benchmark-SET >> "$HI_LOG_FILE-SET.log"
test/hiredis/benchmark-GET >> "$HI_LOG_FILE-GET.log"
test/hiredis/benchmark-DEL >> "$HI_LOG_FILE-DEL.log"

# 印出 log file 最後兩行（執行時間 和 記憶體用量）

echo -e "\n"
echo "Hirebis (my project) performance"
echo -e "\n"
echo "============ SET =============="
tail -n 2 "$MY_LOG_FILE-SET.log"
echo -e "\n"
echo "============ GET =============="
tail -n 2 "$MY_LOG_FILE-GET.log"
echo -e "\n"
echo "============ DEL =============="
tail -n 2 "$MY_LOG_FILE-DEL.log"
echo -e "\n"

echo "Hiredis performance"
echo -e "\n"
echo "============ SET =============="
tail -n 2 "$HI_LOG_FILE-SET.log"
echo -e "\n"
echo "============ GET =============="
tail -n 2 "$HI_LOG_FILE-GET.log"
echo -e "\n"
echo "============ DEL =============="
tail -n 2 "$HI_LOG_FILE-DEL.log"
echo -e "\n"