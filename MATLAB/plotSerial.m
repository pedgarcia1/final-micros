
%Create a serial object 
close all
clear all
instrreset  %(Instrument Reset)
% fclose(instrfindall)

ser = serial('COM5');        % Windows port style
% ser = serial('/dev/ttyUSB0');   % Linux port style

% set object properties
ser.timeout=3;
set(ser, 'Terminator', 10); % set communication string to end on ASCII 13
% Ascii 13->CR  Ascii 10-LF (default)
set(ser, 'BaudRate', 9600);
set(ser, 'StopBits', 1);    
set(ser, 'DataBits', 8);
set(ser, 'Parity', 'none');

reply = 'X';
while (reply~='C' && reply~='V')
    reply = input("Select display mode [V/C]: ","s");
end

fopen(ser);             % open the serial port connection
fprintf(ser,"%c",reply);
answer= fscanf(ser);    % wait for answer o timeout what comes first
disp(answer) % show  response  

figure
hold off
if reply == 'C'
    ylabel("Datos en crudo")
    ylim([0 1023])
elseif reply == 'V'
    ylabel("Voltaje [V]")
    ylim([0 3.66])
end
data = [];
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

    plot(data);
    title(sprintf("Tiempo entre datos: %f s",tStop))

    drawnow
    %     counter = counter + 1;
    if continuar == false
        break
    end
end

fclose(ser);            % Close Port
   
% Notes: 
% 
%     Example: Get object properties


% >> get(ser,'Port')  % get Port properties
% o bien
% >> ser.Port
% ans =
% 
% /dev/ttyS0
%    

% >>ser         % view all properties
% 
%    Serial Port Object : Serial-/dev/ttyS0
% 
%    Communication Settings 
%       Port:               /dev/ttyS0
%       BaudRate:           9600
%       Terminator:         'LF'
% 
%    Communication State 
%       Status:             closed
%       RecordStatus:       off
% 
%    Read/Write State  
%       TransferStatus:     idle
%       BytesAvailable:     0
%       ValuesReceived:     0
%       ValuesSent:         6


% Terminators 
% you can set msg terminator for RX and TX default is 0x0A (LF)
% you can change it using "set(ser, 'Terminator', T)" where T the desired 
% Terminator can be 0-127 (ASCII code or character) or CR/LF or LF/CR
% see this link for more info:
% http://www.mathworks.com/help/matlab/matlab_external/terminator.html
% 


% How reload a mfile cached version 

% The ability to automatically refresh the cached version of a MATLAB file when the RUN function
% is used is not available in MATLAB.
% 
% To work around this issue, use the CLEAR function to erase the version of the script that is in memory. 
% If the script is named 'myscript.m', enter the following at the MATLAB command prompt:
% 
% clear myscript
% The script will then be reloaded when the RUN function is called again.

% For Linux users GtkTerm can me used to debug the serial output from
% matlab

%%%%%% Very Important %%%%%%%%%

% It is very common that after we open a serial port if we stop the code (ctrl-C) the port
% is still open. So if we try to reopen it the operation fails 
% Moreover if the serial port is an usb like device if we disconect and reconect it 
% a new port wil be assigned to it while the previuos port is somewhere taken by the 
% matlab system. closing and reopen matlab is a usual workaround but not very usefull
% since we must wait that matlab opens again

% If this is the case try the folowing

% instrreset  %(Instrument Reset)

% at matlab CLI


% Reference See http://www.mathworks.com/help/instrument/instrreset.html 
 
% Dany!!

  






   


   
