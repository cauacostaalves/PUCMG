%-----------------------------------------------------------
%     SIMULACAO DE UM SISTEMA FUZZY QUE RESPONDE SOBRE
%     O CONSUMO DE UM CARRO EM FUNÇÃO DE SUA VELOCIDADE
%     E DA ACELERAÇÃO.              
%-----------------------------------------------------------

clear;
clc;
close all;

vel = input('Entre com a velocidade de 0 a 100 Km/h: ');
ace = input('Entre com o percentual de aceleração 0 a 100 %: ');
t   = 0:1:100;  % Universo de Discurso.

v_baixo = [ones(1,20) 1:-1/20:0 zeros(1,60)];
v_medio = [zeros(1,20) 0:1/20:1 ones(1,20) (1-1/20):-1/20:0 zeros(1,20)];
v_alto  = [zeros(1,60) 0:1/20:1 ones(1,20)];

fp_v  =  [v_baixo' v_medio' v_alto'];
figure(1);
plot(t,fp_v),grid;
title('FUNÇÕES DE PERTINÊNCIA DA VELOCIDADE - (baixo, médio, alto)');
text(8,0.55,'baixo'),text(45,0.55,'medio'),text(85,0.55,'alto');
ylabel('GRAU DE PERTINÊNCIA');
xlabel('UNIVERSO DO DISCURSO');

a_baixo = [ones(1,20) 1:-1/30:0 zeros(1,50)];
a_medio = [zeros(1,20) 0:1/30:1 (1-1/30):-1/30:0 zeros(1,20)];
a_alto  = [zeros(1,50) 0:1/30:1 ones(1,20)];

fp_a  =  [a_baixo' a_medio' a_alto'];
figure(2);
plot(t,fp_a),grid;
title('FUNÇÕES DE PERTINÊNCIA DA ACELERAÇÃO- (baixo, médio, alto)');
text(8,0.55,'baixo'),text(45,0.55,'medio'),text(85,0.55,'alto');
ylabel('GRAU DE PERTINÊNCIA');
xlabel('UNIVERSO DO DISCURSO');

c_baixo = [1:-1/50:0 zeros(1,50)];
c_medio = [0:1/50:1 (1-1/50):-1/50:0];
c_alto  = [zeros(1,50) 0:1/50:1];

fp_c  =  [c_baixo' c_medio' c_alto'];
figure(3);
plot(t,fp_c),grid;
title('FUNÇÕES DE PERTINÊNCIA DO CONSUMO- (baixo, médio, alto)');
text(8,0.55,'baixo'),text(45,0.55,'medio'),text(85,0.55,'alto');
ylabel('GRAU DE PERTINÊNCIA');
xlabel('UNIVERSO DO DISCURSO');


%------------------------------------------------------------------------------------
% REGRAS NEBULOSAS	- CONSUMO EM FUNÇÃO DA VELOCIDADE E ACELERAÇÃO
%
%							ACELERAÇÃO	
%
%		     		  baixo   medio   alto		 
%   V		       _______________________
%   E       	|	     |	    |	      |
%   L   baixo 	|   M   |   M   |   A   |
%   O       	|_______|_______|_______|
%   C		      |	     |	    |	      |
%   I	  medio	|   B   |   B   |   A   |
%   D		      |_______|_______|_______|
%   A		      |	     |       |	      |
%   D	  alto	|   B   |   M   |   A   |
%   E		      |_______|_______|_______|
%   
%
% r1: se a VELOCIDADE é BAIXA e a ACELERAÇÃO é BAIXA então o CONSUMO é MEDIO.
% r2: se a VELOCIDADE é BAIXA e a ACELERAÇÃO é MÉDIA então o CONSUMO é MEDIO.
% r3: se a VELOCIDADE é BAIXA e a ACELERAÇÃO é ALTA  então o CONSUMO é ALTO.
% 


%r4: se a VELOCIDADE é MÉDIA e a ACELERAÇÃO é BAIXA então o CONSUMO é BAIXO.
% r5: se a VELOCIDADE é MÉDIA e a ACELERAÇÃO é MÉDIA então o CONSUMO é BAIXO.
% r6: se a VELOCIDADE é MÉDIA e a ACELERAÇÃO é ALTA  então o CONSUMO é ALTO.
% r7: se a VELOCIDADE é ALTA  e a ACELERAÇÃO é BAIXA então o CONSUMO é BAIXO.
% r8: se a VELOCIDADE é ALTA  e a ACELERAÇÃO é MÉDIA então o CONSUMO é MEDIO.
% r9: se a VELOCIDADE é ALTA  e a ACELERAÇÃO é ALTA  então o CONSUMO é ALTO.
%
%------------------------------------------------------------------------------------
c1  =  min(c_medio,min(v_baixo(vel+1),a_baixo(ace+1)));

%c1  =  min(c_medio,min(v_baixo(vel+1),a_baixo(ace+1)));
c2  =  min(c_medio,min(v_baixo(vel+1),a_medio(ace+1)))
c3 =  min(c_alto,min(v_baixo(vel+1),a_alto(ace+1)));
c4  =  min(c_baixo,min(v_medio(vel+1),a_baixo(ace+1)));
c5  =  min(c_baixo,min(v_medio(vel+1),a_medio(ace+1)));
c6  =  min(c_alto,min(v_medio(vel+1),a_alto(ace+1)));
c7  =  min(c_baixo,min(v_alto(vel+1),a_baixo(ace+1)));
c8  =  min(c_medio,min(v_alto(vel+1),a_medio(ace+1)));
c9  =  min(c_alto,min(v_alto(vel+1),a_alto(ace+1)));

c   =  max(max(max(max(max(max(max(max(c1,c2),c3),c4),c5),c6),c7),c8),c9);
c1(1)
c2(1)
c3(1)
c4(1)
c5(1)
c6(1)
c7(1)
c8(1)
c9(1)
c
pause

figure(4)
plot(t,c),grid;
title('ÁREA RESULTANTE');
ylabel('GRAU DE PERTINÊNCIA');
xlabel('UNIVERSO DO DISCURSO');

%---------------------------------------------------------
%         DEFUZZIFICACAO POR CENTER AREA METHOD (COA)
%----------------------------------------------------------
   consumo  = sum(c.* t)/sum(c)% Saída defuzzyficada.
   pause

%---------------------------------------------------------------
%  GERAÇÃO DA SUPERFÍCIE CONSUMO  X  VELOCIDADE E ACELERAÇÃO
%---------------------------------------------------------------
linha = 0;
for vel = 0:1:100;
  linha = linha + 1; 
  coluna = 0;
  for ace = 0:1:100;
    coluna = coluna + 1;
  	 c1  =  min(c_medio,min(v_baixo(vel+1),a_baixo(ace+1)));
	 c2  =  min(c_medio,min(v_baixo(vel+1),a_medio(ace+1)));
	 c3  =  min(c_alto,min(v_baixo(vel+1),a_alto(ace+1)));
	 c4  =  min(c_baixo,min(v_medio(vel+1),a_baixo(ace+1)));
	 c5  =  min(c_baixo,min(v_medio(vel+1),a_medio(ace+1)));
	 c6  =  min(c_alto,min(v_medio(vel+1),a_alto(ace+1)));
	 c7  =  min(c_baixo,min(v_alto(vel+1),a_baixo(ace+1)));
	 c8  =  min(c_medio,min(v_alto(vel+1),a_medio(ace+1)));
	 c9  =  min(c_alto,min(v_alto(vel+1),a_alto(ace+1)));
	 c   =  max(max(max(max(max(max(max(max(c1,c2),c3),c4),c5),c6),c7),c8),c9);
    consumo(linha,coluna)  = sum(c.* t)/sum(c);
  end;
end;

figure(5);
mesh(consumo);
xlabel('ACELERAÇÃO');
ylabel('VELOCIDADE');
zlabel('CONSUMO');