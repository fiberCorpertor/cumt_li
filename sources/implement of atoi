int Atoi(string str) 
{
        // 很多异常情况要考虑到，如下：
        //1. 字串为空或者全是空格，返回0； 
        //2. 字串的前缀空格需要忽略掉；
        //3. 忽略掉前缀空格后，遇到的第一个字符，如果是‘+’或‘－’号，继续往后读；如果是数字，则开始处理数字；如果不是前面的2种，返回0；
        //4. 处理数字的过程中，如果之后的字符非数字，就停止转换，返回当前值；
        //5. 在上述处理过程中，如果转换出的值超出了int型的范围，就返回int的最大值或最小值
        
        /*long long sum = 0;
        for (int i = str.size() - 1; i >= 0; i--)
        {
            int tmp = str[i] - 48;
            for (int j = 0; j < str.size() - 1 - i; j++)
            {
                tmp = tmp * 10;
            }
            sum += tmp;
        }*/
        
        if (str.empty())
            return 0;
        
        int i = 0;
    		while (str[i] != '\0' && str[i] == ' ')
    		  ++i;
    		
    		if (str[i] == '\0')
    		  return 0;
    		
    		int max = 0x7fffffff;  
    		int min = 0x80000000;
    		int signal = 1; // 表示符号
    		
    		// 处理+、-号
    		if (str[i] == '+')
    		{
    			signal = 1;
    			++i;  
    		}
    		else if (str[i] == '-')  
    		{
    			signal = -1;
    			++i;
    		}
    		  
    		// 转换整数
    		long long sum = 0;
    		while (str[i] != '\0')
    		{
    			if (str[i] >= '0' && str[i] <= '9')
    				 sum = sum * 10 + signal * (str[i] - '0');
    			else 
    				return sum;
    			if (sum > max || sum < min)   // 溢出处理
    				return sum > 0 ? max : min;
    			++i;
    		}
        return sum;
    }
