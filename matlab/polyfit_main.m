clear;
clc;
close all;

y_raw = [0.6 0.75 0.75 0.1 0.1];
% y_raw = [0 0.75 0 0 0];
y_raw = [0.8 0.85 0.75 0.9 0.86 0.1 0.1 0.1];
x_raw = 1:length(y_raw);
x_continous = 1:0.01:length(y_raw);

p = polyfit(x_raw,y_raw,4);
der = polyder(p)
y_continous = polyval(p, x_continous);

hold on;
plot(x_raw, y_raw);
hold on;
plot(x_continous, y_continous);


