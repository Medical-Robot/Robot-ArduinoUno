clear;
clc;
close all;

y_raw = [0.6 0.75 0.75 0.1 0.1];
y_raw = [0.8 0.85 0.75 0.9 0.86 0.1 0.1 0.1];
x_raw = 0:length(y_raw)-1;
x_continous = 0:0.01:length(y_raw)-1;
y_continous = spline(x_raw, y_raw, x_continous);

hold on;
plot(x_raw, y_raw, 'bo');
plot(x_continous, y_continous);
