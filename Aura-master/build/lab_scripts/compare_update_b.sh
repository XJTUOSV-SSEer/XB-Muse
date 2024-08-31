cd ..
# cmake ..
# make clean
make

result_line=""
sum=0

echo ""
echo "入参 :  4 $i"
for k in {1..10}
do
    # 执行程序并获取返回值
    # 入参： 测试数 数据集 撤销数
    result=$(./SSETest 4)
    echo "第 $k 次执行，结果为: $result"
    # 将返回值加到总和中
    result_float=$(echo $result | awk '{print $1}')
    sum=$(echo "$sum + $result_float" | bc)
done

# 计算平均值
average=$(echo "$sum / 10" | bc -l)
echo "平均值为: $average"

result_line+="$(echo "scale=5; $sum / 10" | bc), "

echo $result_line > ./lab_scripts/result/result_compare_update_b.csv