clear;
clc;
close all;

y_raw = [0.6 0.75 0.75 0.1 0.1];
x_raw = 0:length(y_raw)-1;
x_continous = -1:0.01:5;

p = polyfit(x_raw, y_raw, 4);
% p = [0.075062 -0.533121 0.999449 -0.391220 0.599973]
der = polyder(p);
real_roots_p = roots(der);
% real_roots_p = [1.134337 -0.183438 -0.183438];

extrema_values = polyval(p, real_roots_p);

fprintf('Minim: x = %.4f, valoare = %.4f\n', real_roots_p(extrema_values == min(extrema_values)), min(extrema_values));
fprintf('Maxim: x = %.4f, valoare = %.4f\n', real_roots_p(extrema_values == max(extrema_values)), max(extrema_values));


y_continous = polyval(p, x_continous);
hold on;
plot(x_raw, y_raw, 'bo');
plot(x_continous, y_continous);
plot(real_roots_p, extrema_values,'rx');