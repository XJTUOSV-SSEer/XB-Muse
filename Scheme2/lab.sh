clear
# make clean
make SGX_MODE=HW SGX_DEBUG=1

result_line=""
# 执行程序10次，将i作为入参
for i in {1..10}
do
  sum=0
  # 取10次的平均值
  for j in {1..10}
  do
    # 执行你的程序并获取返回值
    result=$(./cryptoTestingApp $i)
    echo "第 $i 次执行，结果为: $result"
    # 将返回值加到总和中
    sum=$((sum + result))
  done
  result_line+="$((sum/10)) "
done
echo $result_line >> result.csv

# 计算平均值
average=$(echo "$sum / 10" | bc -l)

echo "平均值为: $average"