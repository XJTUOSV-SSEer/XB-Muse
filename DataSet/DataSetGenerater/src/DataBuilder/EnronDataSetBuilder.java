package DataBuilder;

import java.io.*;
import java.util.*;

public class EnronDataSetBuilder extends AbstractDataSetBuilder{

    public Map<String,Set<String>> getDataSetMap(String filePath){
        Map<String,Set<String>> map = new HashMap<>();
        try {
            Scanner scanner = new Scanner(new File(filePath), "UTF-8");
            while(scanner.hasNextLine()){
                String s = scanner.nextLine();
                String[] kv = s.split("\t");
                if(!map.containsKey(kv[0])){
                    map.put(kv[0],new HashSet<>());
                }
                map.get(kv[0]).add(kv[1]);
            }
        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        }
        return map;
    }

    public void print(Map<String,Set<String>> DataSetMap,int minLen){
        DataSetMap.entrySet().stream()
                .sorted((e1,e2) -> e2.getValue().size() - e1.getValue().size())
                .filter(e -> e.getValue().size() > minLen)
                .forEach(e -> System.out.println(e.getKey() + " " + e.getValue().size()+ " " + e.getValue()));
        int totalKVNum = 0;
        for(Set<String> e: DataSetMap.values()){
            totalKVNum += e.size();
        }
//        System.out.println("total size: "+totalKVNum);
    }

    public void generate(Map<String,Set<String>> DataSetMap,String filePath){
        mapToFile(DataSetMap,filePath);
    }
}
