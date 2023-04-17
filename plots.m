hold on 
grid on

x2= table2array(winloss(:,1));
x1= table2array(winloss(:,2));
x3= table2array(winloss(:,3));
x4= table2array(winloss(:,4));

len= length(x1);

plot(1:len, x1,'b')
plot(1:len, x2,'k')
plot(1:len, x3,'g')
plot(1:len, x4,'r')
xlabel('Hands')
ylabel('Wins')
title('Algo Holdem')
legend('generic','opstop','expexp','monte')

