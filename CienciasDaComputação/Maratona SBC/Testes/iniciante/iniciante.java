import java.util.Scanner;

public class iniciante{
    public static void main(String[] args){
        Scanner sc = new Scanner(System.in);
        int t = 0;
        int acc = 0;
        for(int i=0; i<4; i++){
            t = sc.nextInt();
            acc += t;
        }
        System.out.println(acc-3);
        sc.close();
    }
}