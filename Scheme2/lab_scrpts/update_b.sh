clear
cd ..
make clean
make SGX_MODE=HW SGX_DEBUG=1

for i in {1..4}
do
  result_line=""

  for j in {6..10}
  do
    echo ""
    echo "入参 : 6 $j $i"

    sum=0
    # 执行程序10次，将j作为入参
    # 取10次的平均值
    for k in {1..10}
    do
      # 执行你的程序并获取返回值
      result=$(./cryptoTestingApp 6 $j $i)
      echo "第 $k 次执行，结果为: $result"
      # 将返回值加到总和中
      result_float=$(echo $result | awk '{print $1}')
      sum=$(echo "$sum + $result_float" | bc)
    done
    result_line+="$(echo "scale=5; $sum / 10" | bc), "

    # 计算平均值
    average=$(echo "$sum / 10" | bc -l)
    echo "平均值为: $average"
  done
  echo $result_line >> ./lab_scrpts/result_update_d.csv
done