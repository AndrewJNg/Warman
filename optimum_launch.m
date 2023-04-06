fclose all; close all; clear all; clc;

u = 300; % cm/s
theta = 45; % launch angle

d_x = 60; % horizontal distance
d_y = 60; % vertical distance

a = -9.81;

%horizontal plane
%t = @(u,theta) d_x ./ (u.*cos(theta));
t =  d_x ./ (u.*cos(theta));


%vertical plane
%f = @(u,t) u.*sin(t)+1/2.*(a).*t.^2 - d_y;
f = @(u) u.*sin(t)+1/2.*(a).*t.^2 - d_y;
plot(u,f(u))
f(u)
%plot(theta,f(u,t(u,theta)))


%fzero(f,1)

%%
fclose all; close all; clear all; clc;
theta_degree = 80; %degree
x=0.6; % m
y=0.6; % m

a = -9.81; %m/s^2
theta_rad = theta_degree/180*pi; % convert angle to rad

% horizontal motion
ut = x/cos(theta_rad);

% vertical motion
f = @(t) a/2 * t^2 + ut*sin(theta_rad) + y;

t = fzero(f,8) % time taken to launch 
u = ut/t %  initial speed to launch






