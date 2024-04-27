clear;
clc;
close all;

y_raw = [0.6 0.75 0.75 0.1 0.1];
% y_raw = [0 0.75 0 0 0];
x_raw = 0:length(y_raw)-1;
x_continous = -1:0.01:5;

p = polyfit(x_raw,y_raw,4);
p = [0.075062 -0.533121 0.999449 -0.391220 0.599973]
der = polyder(p)
y_continous = polyval(p, x_continous);

hold on;
plot(x_raw, y_raw);
hold on;
plot(x_continous, y_continous);


