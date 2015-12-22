class Solution {
public:
    vector<string> summaryRanges(vector<int>& nums) {
        std::vector<std::string> result;
        std::string range;
        char tmp[24];
        
        if (nums.empty()){
            return result;
        }else if (nums.size() == 1){
            sprintf(tmp, "%d", nums[0]);
            std::string aaa(tmp);
            range.append(aaa);
            result.push_back(range);
            return result;
        }
        
        // total 用来保存当前累积了多少个值，比如0,1,2,5，从0-2累积了3个
        int begin = nums[0], cur = nums[0], total = 0;
        sprintf(tmp, "%d", begin);
        std::string ddd(tmp);
        range.append(ddd);
        total = 1;

        for (int i = 1; i < nums.size(); i++){
            if (nums[i] - 1 == cur){
                total++;
                cur = nums[i];
                if (i == nums.size() - 1){
                    range.append("->");
                    memset(tmp, 0, 24);
                    sprintf(tmp, "%d", cur);
                    std::string aaa(tmp);
                    range.append(aaa);
                    result.push_back(range);
                }
            }
            else{
                if (total != 1){ // 当累积的数量为1时，表示是独立的区间，不需要加->
                    range.append("->");
                    memset(tmp, 0, 24);
                    sprintf(tmp, "%d", cur);
                    std::string bbb(tmp);
                    range.append(bbb);
                }
                result.push_back(range);
                range.clear();
                
                begin = nums[i];
                cur = nums[i];
                total = 1;
                
                memset(tmp, 0, 24);
                sprintf(tmp, "%d", cur);
                std::string eee(tmp);
                range.append(eee);
                if (i == nums.size() - 1 )
                {
                    result.push_back(range);
                }
            }
        }
        return result;
    }
};
