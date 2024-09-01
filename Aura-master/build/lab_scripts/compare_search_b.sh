cd ..
# cmake ..
# make clean
make

sum1=0
sum2=0

echo ""
echo "入参 :  2"
for k in {1..10}
do
    # 执行程序并获取返回值
    # 入参： 测试数 数据集 撤销数
    result=$(./SSETest 2)
    read num1 num2 <<< $result
    echo "第 $k 次执行，结果为: $num1 $num2"
    # 将返回值加到总和中
    result_float1=$(echo $num1| awk '{print $1}')
    result_float2=$(echo $num2| awk '{print $1}')
    sum1=$(echo "$sum1 + $result_float1" | bc)
    sum2=$(echo "$sum2 + $result_float2" | bc)
done

# 计算平均值
average1=$(echo "$sum1 / 10" | bc -l)
average2=$(echo "$sum2 / 10" | bc -l)
echo "平均值1为: $average1"
echo "平均值2为: $average2"

# result_line+="$(echo "scale=5; $sum / 10" | bc), "

# echo $result_line > ./lab_scripts/result/result_compare_update_b.csv