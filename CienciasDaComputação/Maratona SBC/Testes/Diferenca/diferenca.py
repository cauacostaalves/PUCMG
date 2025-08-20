A = B = C = D = 0   # todas começam com 0
A = int(input())
B = int(input())
C = int(input())
D = int(input())
DIFERENCA = (A * B - C * D)
print("DIFERENCA = " , DIFERENCA)

A = B = C = D = 0   
A, B, C, D = list(map(int, input().split()))
DIFERENCA = (A * B - C * D)
print("DIFERENCA = " , DIFERENCA)