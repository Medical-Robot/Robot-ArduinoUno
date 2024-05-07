clear;
clc;
close all;

y_raw = [0.6 0.75 0.75 0.1 0.1];
y_raw = [0.8 0.85 0.75 0.9 0.86 0.1 0.1 0.1];
x_raw = 1:length(y_raw);
x_continous = 0:0.01:length(y_raw)+1;

p = polyfit(x_raw, y_raw, 4);
roots_p = roots(polyder(p));

minima = fnmin(p, roots_p);
maxima = fnmin(-p, roots_p);

fprintf('Minim: x = %.4f, valoare = %.4f\n', minima(1), minima(2));
fprintf('Maxim: x = %.4f, valoare = %.4f\n', maxima(1), maxima(2));

y_continous = polyval(p, x_continous);
hold on;
plot(x_raw, y_raw, 'bo');
plot(x_continous, y_continous);
plot(minima(1), minima(2), 'rx'); % Plot minimum
plot(maxima(1), maxima(2), 'rx'); % Plot maximum

