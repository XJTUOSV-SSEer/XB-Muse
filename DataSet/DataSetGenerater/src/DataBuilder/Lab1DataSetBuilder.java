package DataBuilder;

import java.util.*;

public class Lab1DataSetBuilder extends AbstractDataSetBuilder {

    private static final int IND_MIN = 1;
    private static final int IND_MAX = 1000000;
    public void generateDataSet(String filePath,int toSearchSize,int DatasetSize) {
        Map<String,Set<String>> dataSetMap = new HashMap<>();
        String key = generateString(1, 10);
        Set<String> set = new HashSet<>();
        dataSetMap.put(key,set);
        System.out.println("target key : " + key);
        while(set.size() < toSearchSize){
            //文件描述符范围
            set.add(String.valueOf(generateInt(IND_MIN,IND_MAX)));
        }
        int cnt = set.size();
//        System.out.println(set.size());
        while(cnt < DatasetSize){
            //这个key对应的ind个数
            int keySize = generateInt(10,1000);
            do {
                key = generateString(1, 10);
            }while(set.contains(key));
            set = new HashSet<>();
            while (set.size() < keySize){
                set.add(String.valueOf(generateInt(IND_MIN,IND_MAX)));
            }
            cnt += keySize;
            dataSetMap.put(key,set);
        }
        mapToFile(dataSetMap,filePath);
    }
}
