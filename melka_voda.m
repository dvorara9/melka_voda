clear all; close all; clc;
%% skript pro pocitani vysky hladiny melke vody
% Radim Dvorak
% 18.11.2020* - 8.12.2020?

%____________________________PREPROCESSOR__________________________________
% vstupy
    ndx = 100; % pocet uzlu na x
    Termi = 1; % doba reseni
    Cour = 0.1; % courantovo c. ulohy (1 = mez stability pro linearni ulohu, zde je stabilita cca na 0,1)
    h_prehrady = 2; % pocatecni vyska hladiny prehrady
    h_reky = 1; % pocatecni vyska hladiny reky za prehradou
    l_prehrady = 10; % delka prehrady
    l_reky = 10; % delka reky za prehradou
    u_prehrady = 0; % horizontalni rychlost prehrady
    u_reky = 1; % horizontalni rychlost reky
%     
%     ndx = 120; % pocet uzlu na x
%     Termi = 10; % doba reseni
%     Cour = 0.1; % courantovo c. ulohy (1 = mez stability pro linearni ulohu, zde je stabilita cca na 0,1)
%     h_prehrady = 10; % pocatecni vyska hladiny prehrady
%     h_reky = 1; % pocatecni vyska hladiny reky za prehradou
%     l_prehrady = 20; % delka prehrady
%     l_reky = 100; % delka reky za prehradou
%     u_prehrady = 0; % horizontalni rychlost prehrady
%     u_reky = 1; % horizontalni rychlost reky

% konstanty
    g = 9.81; % tihove zrychleni
    
% dopoctene parametry
    dx = (l_prehrady + l_reky)./ndx; 
    dt = Cour .* dx;
    ndt = round(Termi./dt);
    coo = [0:dx:l_reky+l_prehrady-dx] - l_prehrady;

%______________________________SOLVER______________________________________
    [arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9] = deal(num2str(ndx),num2str(Termi),num2str(Cour),num2str(h_prehrady),num2str(h_reky),num2str(l_prehrady),num2str(l_reky),num2str(u_prehrady),num2str(u_reky));
    CommandStr = ['main_fcni.exe ' arg1 ' ' arg2 ' ' arg3 ' ' arg4 ' ' arg5 ' ' arg6 ' ' arg7 ' ' arg8 ' ' arg9];
    system(CommandStr);
    while exist('konec.txt', 'file') == 0
        pause(0.01)
    end
    delete 'konec.txt'
%_____________________________POSTPROCESSOR________________________________
% pracovni animace, prepisu podle toho, v jakym formatu bude vystup c++
    H = dlmread('vyska_hladiny.txt');
    U = dlmread('horizontalni_rychlost.txt');

    % limity os
        ylimH = [min(min(H))-0.1.*abs(min(min(H))-max(max(H))) max(max(H))+0.1.*abs(min(min(H))-max(max(H)))];
        ylimU = [min(min(U))-0.1.*abs(min(min(U))-max(max(U))) max(max(U))+0.1.*abs(min(min(U))-max(max(U)))];
    % plot
        again = 'ANO';
        while strcmp(again,'ANO')
            for n = 1:ndt
                subplot(2,1,1)
                plot(coo,H(n,:))
                title(['vyska hladiny ' num2str(n/ndt*Termi) ' s (' num2str(round(n./ndt.*100)) '%)'])
                xlabel('x [m]')
                ylabel('h [m]')
                ylim(ylimH)
                subplot(2,1,2)
                plot(coo,U(n,:))
                title(['horizontalni rychlost ' num2str(n/ndt*Termi) ' s (' num2str(round(n./ndt.*100)) '%)'])
                xlabel('x [m]')
                ylabel('u [m/s]')
                ylim(ylimU)
                pause(0.1)
            end
            again = questdlg('Zopakovat animaci?','animace','ANO','NE','NE');
        end

        
    