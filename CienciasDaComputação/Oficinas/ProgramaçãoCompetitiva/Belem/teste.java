/*
class Solution {
    public int climbStairs(int n) {
        int[] v = new int [n+2];
        v[0] = 0;
        v[1] = 1;
        if(n <= 1){
            return v[n];
        }
        for(int i = 2; i<=n+1;i++){
            v[i] = v[i-1] + v[i-2];
        }
        return v[n+1];
    }
}
*/
//complexidade = n memeoria n
class Solution {
    public int climbStairs(int n) {
        if(n <=1){
            return n;
        }
        int a =0;
        int b =1;
        for(int i = 2; i<=n+1;i++){
            int temp = b;
            b = b+a;
            a = temp;
        }
        return b;
    }
}
// complexidade n e memoria 1

