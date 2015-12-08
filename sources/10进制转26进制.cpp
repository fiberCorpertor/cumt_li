// 注意这里的减1操作

class Solution {
public:
    string convertToTitle(int n) {
        string ret = "";
        while(n)
        {
            ret = (char)((n-1) % 26 + 'A') + ret;
            n = (n-1) / 26;
        }
        return ret;
    }
};
