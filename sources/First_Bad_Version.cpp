// 毫无疑问，使用二分查找法
// 尤其需要注意的一点：如果你把ver = (low + high)/2; 那就会出现超时错误，因为直接相加可能溢出，
// 另外，else 处是low = ver + 1，不是low = ver
class Solution {
public:
    int firstBadVersion(int n) {
        int low = 1;
        int high = n;
        int ver = 0;
        while (low < high) {
            ver = low + (high - low) / 2;
            if (isBadVersion(ver)) {
                high = ver;
            } else {
                low = ver + 1;
            }
        }
        return low;
    }
};
