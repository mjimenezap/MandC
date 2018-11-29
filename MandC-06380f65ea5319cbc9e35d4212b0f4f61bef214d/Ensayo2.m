ensayo=[ % Línea añadida.
45, 312;
45, 306;
45, 320;
45, 320;
45, 315;
45, 320;
45, 330;
45, 309;
45, 320;
45, 309;
45, 318;
45, 318;
45, 309;
45, 318;
45, 315;
45, 318;
45, 325;
45, 312;
45, 325;
45, 325;
45, 315;
135, -67;
135, -89;
135, -115;
135, -67;
135, -81;
135, -71;
135, -87;
135, -87;
135, -65;
135, -45;
135, -100;
135, -64;
135, -61;
135, -100;
135, -89;
135, -105;
135, -100;
135, -96;
135, -109;
135, -98;


]; % Línea añadida.

% Separa los datos del vector ensayo en dos vectores: u para la entrada
% e y para la salida. Genera además un vector de tiempo y representa
% gráficamente los datos para ver fácilmente si el ensayo es bueno
% para identificar la planta.

t_s = 0.001; % periodo de muestreo = 1 ms

% Crea el vector de tiempos
tam_vec = size(ensayo);
t = 0:t_s:(tam_vec(1)-1)*t_s;

%Obtiene los vectores de entrada y salida.
u = ensayo(:,1);
y = ensayo(:,2);

% Y los representa gráficamente
plot(t,u,t,y);

% por último elimina el offset de la señal de salida, tal como lo requiere
% el estimador del pidTuner
y_sin_offset = y - y(1);
figure(2);
plot(t,y_sin_offset);