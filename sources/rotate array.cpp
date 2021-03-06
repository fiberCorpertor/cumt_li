// 整个solution的关键在于整除求余 % 运算符的合理利用

// method 1
// Make an extra copy and then rotate.
// Time complexity: O(n). Space complexity: O(n)
// 这么理解：下标为0的转移到下标为3，下标为1的转移到下标为4，以此类推，下标为i的转移到下标为(i+k)%n
class Solution 
    {
    public:
        void rotate(int nums[], int n, int k) 
        {
            if ((n == 0) || (k <= 0))
            {
                return;
            }

            // Make a copy of nums
            vector<int> numsCopy(n);
            for (int i = 0; i < n; i++)
            {
                numsCopy[i] = nums[i];
            }

            // Rotate the elements.
            for (int i = 0; i < n; i++)
            {
                nums[(i + k)%n] = numsCopy[i];
            }
        }
    };
    
// method 2
// Reverse the first n - k elements, the last k elements, and then all the n elements
// Time complexity: O(n). Space complexity: O(1)
class Solution 
    {
    public:
        void rotate(int nums[], int n, int k) 
        {
            k = k%n;

            // Reverse the first n - k numbers.
            // Index i (0 <= i < n - k) becomes n - k - i.
            reverse(nums, nums + n - k);

            // Reverse tha last k numbers.
            // Index n - k + i (0 <= i < k) becomes n - i.
            reverse(nums + n - k, nums + n);

            // Reverse all the numbers.
            // Index i (0 <= i < n - k) becomes n - (n - k - i) = i + k.
            // Index n - k + i (0 <= i < k) becomes n - (n - i) = i.
            reverse(nums, nums + n);
        }
    };
    
// methon 3
// Swap the last k elements with the first k elements
// Time complexity: O(n). Space complexity: O(1)
class Solution 
{
public:
    void rotate(int nums[], int n, int k) 
    {
        for (; k = k%n; n -= k, nums += k)
        {
            // Swap the last k elements with the first k elements. 
            // The last k elements will be in the correct positions
            // but we need to rotate the remaining (n - k) elements 
            // to the right by k steps.
            for (int i = 0; i < k; i++)
            {
                swap(nums[i], nums[n - k + i]);
            }
        }
    }
};
