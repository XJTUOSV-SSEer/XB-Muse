package DataBuilder;

import java.io.*;
import java.util.Map;
import java.util.Set;

public abstract class AbstractDataSetBuilder {

    void mapToFile(Map<String, Set<String>> dataSet, String filePath) {
        File file = new File(filePath);
        if(!file.exists()){
            try {
                file.createNewFile();
            } catch (IOException e) {
                e.printStackTrace();
                return ;
            }
        }
        FileWriter writer = null;
        try  {
            writer = new FileWriter(filePath, false);
            // 通过使用FileWriter并设置append为false，可以清空文件内容
            writer.write("");
        } catch (IOException e) {
            e.printStackTrace();
            return ;
        }
        StringBuilder sb = new StringBuilder();
        for(Map.Entry<String,Set<String>> entry: dataSet.entrySet()){
            sb.delete(0, sb.length());
            String key = entry.getKey();
            Set<String> value = entry.getValue();
            sb.append(key);
            for(String v: value){
                sb.append(" ");
                sb.append(v);
            }
            sb.append('\n');
            try {
                writer.write(sb.toString());
                writer.flush();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }

    protected String generateString(int minLen, int maxLen){
        int len = (int) (Math.random() * (maxLen - minLen + 1) + minLen);
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < len; i++) {
            char c = (char) (Math.random() * 26 + 'A');
            sb.append(c);
        }
        return sb.toString();
    }

    protected int generateInt(int min, int max) {
        return (int) (Math.random() * (max - min + 1) + min);
    }
}
