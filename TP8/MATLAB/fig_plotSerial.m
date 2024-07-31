close all; clear all

data = [];
%Create a serial object 
instrreset  %(Instrument Reset)

ser = serial('COM1');        % Windows port style
% ser = serial('/dev/ttyUSB0');   % Linux port style

% Crear una figura
fig = figure('Position', [200, 200, 750, 550]);

% Crear un axis para el gráfico
ax = axes('Parent', fig, 'Position', [0.1, 0.3, 0.8, 0.6]);

% Crear un texto
textLabel = uicontrol('Style', 'text', 'String', "Seleccionar modo de display [V/C]: ", 'Position', [100, 100, 200, 20]);

% Crear un cuadro de texto
txt = [];
% txt = uicontrol('Style', 'edit', 'Position', [150, 50, 100, 30], 'Callback', {@enviarMicro,txt,ax,ser,data});
txt = uicontrol('Style', 'edit', 'Position', [150, 50, 100, 30]);

% Read and plot data
reply = 'V'; % default
%% PLOT SERIAL
% set object properties
ser.timeout=3;
set(ser, 'Terminator', 10); % set communication string to end on ASCII 13
% Ascii 13->CR  Ascii 10-LF (default)
set(ser, 'BaudRate', 9600);
set(ser, 'StopBits', 1);    
set(ser, 'DataBits', 8);
set(ser, 'Parity', 'none');

% reply = 'X';
% while (reply~='C' && reply~='V')
%     reply = input("Select display mode [V/C]: ","s");
% end

fopen(ser);             % open the serial port connection
fprintf(ser,"%c",reply);
answer= fscanf(ser);    % wait for answer o timeout what comes first
disp(answer) % show  response  

hold off
% data = [];
continuar = true;
line = ''; tStart = tic; tStop = 0;
while true
    aux = fread(ser,1,'char');
%     disp(aux)
    if ~isempty(aux)
    if aux == 10 % \n 
        data = [data,str2double(line)];
        line = [];
        tStop = toc(tStart);
        tStart = tic;
    else
        line = [line,char(aux)];
    end
    end

    plot(ax,data);
    title(ax,sprintf("Tiempo entre datos: %f s",tStop))
   
    reply = get(txt, 'String');
    % Verificar si el texto es diferente de 'C' o 'V'
    if ~strcmp(reply, 'C') && ~strcmp(reply, 'V')
        % Borrar el contenido del cuadro de texto
        set(txt, 'String', '');
    end
    if ~strcmp(reply,'')
        fprintf(ser,"%c",reply);
        answer= fscanf(ser);    % wait for answer o timeout what comes first
        while ~contains(answer,"Modo")
            answer= fscanf(ser);
        end
        disp(answer) % show  response
        data = [];
        set(txt, 'String', '');
    end

    drawnow
    %     counter = counter + 1;
    if continuar == false
        break
    end
end