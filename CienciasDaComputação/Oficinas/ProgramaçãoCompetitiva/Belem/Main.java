import java.util.Scanner;

public class Main {
    static int G = 0;
    static int R = 0;
    static int B = 0;

    public static void TesteGOL(char tmp1, char tmp2) {
        if (tmp1 == 'R') {
            if (tmp2 == 'B') {
                R += 2;
            } else if (tmp2 == 'G') {
                R += 1;
            }
        }
        if (tmp1 == 'G') {
            if (tmp2 == 'R') {
                G += 2;
            } else if (tmp2 == 'B') {
                G += 1;
            }
        }
        if (tmp1 == 'B') {
            if (tmp2 == 'G') {
                B += 2;
            } else if (tmp2 == 'R') {
                B += 1;
            }
        }
    }

    public static void resposta(){
        
        if (B == G && R == G && B == R ) {
            System.out.println("trempate");
        } else if (B > G && B > R) {
            System.out.println("blue");
        } else if (G > B && G > R) {
            System.out.println("green");
        } else if (R > B && R > G) {
            System.out.println("red");
        } else {
            System.out.println("empate");
        }

    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int C = sc.nextInt();
        sc.nextLine();

        while (C > 0) {
            int QTDgols = sc.nextInt();
            sc.nextLine();
            for (int i = 0; i < QTDgols; i++) {
                String tmp = sc.nextLine();
                char tmp1 = tmp.charAt(0);
                char tmp2 = tmp.charAt(2);
                TesteGOL(tmp1, tmp2);
            }

           resposta();

            R = 0;
            G = 0;
            B = 0;
            C--;
        }
        sc.close();
    }

}
