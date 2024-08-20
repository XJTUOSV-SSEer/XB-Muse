clear
cd ..
make clean
make SGX_MODE=HW SGX_DEBUG=1

for i in 13
do
  result_line=""
  for j in {1..5}
  do
      sum=0

      # 取10次的平均值
      echo ""
      echo "入参 ：  4 $i $j"
      for k in {1..10}
      do
        # 执行程序并获取返回值
        # 入参： 测试数 数据集 撤销批数（一批300个）
        result=$(./cryptoTestingApp 4 $i $j)
        echo "第 $k 次执行，结果为: $result"
        # 将返回值加到总和中
        result_float=$(echo $result | awk '{print $1}')
        sum=$(echo "$sum + $result_float" | bc)
      done

      # 计算平均值
      average=$(echo "$sum / 10" | bc -l)
      echo "平均值为: $average"

      result_line+="$(echo "scale=5; $sum / 10" | bc), "
  done

  echo $result_line >> ./lab_scrpts/result_search_d.csv

done