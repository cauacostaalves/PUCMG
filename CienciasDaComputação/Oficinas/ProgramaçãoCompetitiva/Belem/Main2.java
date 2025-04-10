import java.util.*;

public class Main2 {
 
    public static void main(String[] args)  {
 
        Scanner sc = new Scanner(System.in);
        double R = 0;
        double QTDgas = 0;
        double pi = 3.1415;

        R = sc.nextDouble();
        QTDgas = sc.nextDouble();
        
        double resp = (4.0 * pi * (R*R* R)) / 3.0;

        int resposta =(int) (QTDgas/resp);

        System.out.println(resposta);
        
    }
 
}

