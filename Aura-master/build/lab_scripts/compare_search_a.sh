cd ..
cmake ..
make clean
make

result_line=""

for i in enron_partition1 enron_partition2 enron_partition3 enron_partition4 enron_processed
do
    sum=0

    dataSetFilePath="../../DataSet/$i"
    # 取10次的平均值
    echo ""
    echo "入参 :  1 $i"
    for k in {1..10}
    do
        # 执行程序并获取返回值
        # 入参： 测试数 数据集 撤销数
        result=$(./SSETest 1 $dataSetFilePath)
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

echo $result_line > ./lab_scripts/compare_search_a.csv
