#!/bin/bash

generate_random_string() {
    local length="$1"
    tr -cd 'a-zA-Z0-9' < /dev/urandom | head -c "$length"
}

# 生成幾筆資料
num_pairs=40000

for ((i = 1; i <= num_pairs; i++)); do
    key=$(generate_random_string 10)
    value=$(generate_random_string 20)
    echo "SET $key $value" >> test_data_for_SET.txt
    echo -ne " $i 筆SET測試資料已經生成\r"
done

echo -e "\n"

for ((i = 1; i <= num_pairs; i++)); do
    key=$(generate_random_string 10)
    echo "GET $key" >> test_data_for_GET.txt
    echo -ne " $i 筆GET測試資料已經生成\r"
done

echo -e "\n"

for ((i = 1; i <= num_pairs; i++)); do
    key=$(generate_random_string 10)
    echo "DEL $key" >> test_data_for_DEL.txt
    echo -ne " $i 筆DEL測試資料已經生成\r"
done