% y_residues_diffR = zeros(46314450, 1);
% cb_residues_diffR = zeros(46314450, 1);
% cr_residues_diffR = zeros(46314450, 1);
% 
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% %%%%%%%%%% Differential Raster %%%%%%%%%%%
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% counter = 1;
% fp_y_diffR = fopen('../../results/y_residues_diffR.txt');
% while ~feof(fp_y_diffR)
%     line = fgetl(fp_y_diffR);
%     %disp(line);
%     y_residues_diffR(counter) = str2double(line);
%     counter = counter + 1;
%     if mod(counter, 500000) == 0
%         disp(counter)
%     end;
% end;
% fclose(fp_y_diffR);
% 
% counter = 1;
% fp_cb_diffR = fopen('../../results/cb_residues_diffR.txt');
% while ~feof(fp_cb_diffR)
%     line = fgetl(fp_cb_diffR);
%     %disp(line);
%     cb_residues_diffR(counter) = str2double(line);
%     counter = counter + 1;
%     if mod(counter, 500000) == 0
%         disp(counter)
%     end;
% end;
% fclose(fp_cb_diffR);
% 
% counter = 1;
% fp_cr_diffR = fopen('../../results/cr_residues_diffR.txt');
% while ~feof(fp_cr_diffR)
%     line = fgetl(fp_cr_diffR);
%     %disp(line);
%     cr_residues_diffR(counter) = str2double(line);
%     counter = counter + 1;
%     if mod(counter, 500000) == 0
%         disp(counter)
%     end;
% end;
% fclose(fp_cr_diffR);
% 
% save('y_residues_diffR.mat', 'y_residues_diffR');
% save('cb_residues_diffR.mat', 'cb_residues_diffR');
% save('cr_residues_diffR.mat', 'cr_residues_diffR');
% clear y_residues_diffR;
% clear cb_residues_diffR;
% clear cr_residues_diffR;
% 
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% %%%%%%%%%% Differential Central %%%%%%%%%%
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% y_residues_diffC = zeros(46314450, 1);
% cb_residues_diffC = zeros(46314450, 1);
% cr_residues_diffC = zeros(46314450, 1);
% 
% counter = 1;
% fp_y_diffC = fopen('../../results/y_residues_diffC.txt');
% while ~feof(fp_y_diffC)
%     line = fgetl(fp_y_diffC);
%     %%disp(line);
%     y_residues_diffC(counter) = str2double(line);
%     counter = counter + 1;
%     if mod(counter, 500000) == 0
%         disp(counter)
%     end;
% end;
% fclose(fp_y_diffC);
% 
% counter = 1;
% fp_cb_diffC = fopen('../../results/cb_residues_diffC.txt');
% while ~feof(fp_cb_diffC)
%     line = fgetl(fp_cb_diffC);
%     %%disp(line);
%     cb_residues_diffC(counter) = str2double(line);
%     counter = counter + 1;
%     if mod(counter, 500000) == 0
%         disp(counter)
%     end;
% end;
% fclose(fp_cb_diffC);
% 
% counter = 1;
% fp_cr_diffC = fopen('../../results/cr_residues_diffC.txt');
% while ~feof(fp_cr_diffC)
%     line = fgetl(fp_cr_diffC);
%     %%disp(line);
%     cr_residues_diffC(counter) = str2double(line);
%     counter = counter + 1;
%     if mod(counter, 500000) == 0
%         disp(counter)
%     end;
% end;
% fclose(fp_cr_diffC);
% 
% save('y_residues_diffC.mat', 'y_residues_diffC');
% save('cb_residues_diffC.mat', 'cb_residues_diffC');
% save('cr_residues_diffC.mat', 'cr_residues_diffC');
% clear y_residues_diffC;
% clear cb_residues_diffC;
% clear cr_residues_diffC;
% 
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% %%%%%%%%%%%%%%%%%% MuLE %%%%%%%%%%%%%%%%%%
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% y_samples_MuLE = zeros(46314450, 1);
% cb_samples_MuLE = zeros(46314450, 1);
% cr_samples_MuLE = zeros(46314450, 1);
% 
% counter = 1;
% fp_y_MuLE = fopen('../../results/y_samples_MuLE.txt');
% while ~feof(fp_y_MuLE)
%     line = fgetl(fp_y_MuLE);
%     %%disp(line);
%     y_samples_MuLE(counter) = str2double(line);
%     counter = counter + 1;
%     if mod(counter, 500000) == 0
%         disp(counter)
%     end;
% end;
% fclose(fp_y_MuLE);
% 
% counter = 1;
% fp_cb_MuLE = fopen('../../results/cb_samples_MuLE.txt');
% while ~feof(fp_cb_MuLE)
%     line = fgetl(fp_cb_MuLE);
%     %%disp(line);
%     cb_samples_MuLE(counter) = str2double(line);
%     counter = counter + 1;
%     if mod(counter, 500000) == 0
%         disp(counter)
%     end;
% end;
% fclose(fp_cb_MuLE);
% 
% counter = 1;
% fp_cr_MuLE = fopen('../../results/cr_samples_MuLE.txt');
% while ~feof(fp_cr_MuLE)
%     line = fgetl(fp_cr_MuLE);
%     %%disp(line);
%     cr_samples_MuLE(counter) = str2double(line);
%     counter = counter + 1;
%     if mod(counter, 500000) == 0
%         disp(counter)
%     end;
% end;
% fclose(fp_cr_MuLE);
% 
% save('y_samples_MuLE.mat', 'y_samples_MuLE');
% save('cb_samples_MuLE.mat', 'cb_samples_MuLE');
% save('cr_samples_MuLE.mat', 'cr_samples_MuLE');
% clear y_samples_MuLE;
% clear cb_samples_MuLE;
% clear cr_samples_MuLE;
% 
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% %%%%%%%%%%%%%%%%%% orig %%%%%%%%%%%%%%%%%%
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% y_samples_orig = zeros(46314450, 1);
% cb_samples_orig = zeros(46314450, 1);
% cr_samples_orig = zeros(46314450, 1);
% 
% counter = 1;
% fp_y_orig = fopen('../../results/y_samples_orig.txt');
% while ~feof(fp_y_orig)
%     line = fgetl(fp_y_orig);
%     %%disp(line);
%     y_samples_orig(counter) = str2double(line);
%     counter = counter + 1;
%     if mod(counter, 500000) == 0
%         disp(counter)
%     end;
% end;
% fclose(fp_y_orig);
% 
% counter = 1;
% fp_cb_orig = fopen('../../results/cb_samples_orig.txt');
% while ~feof(fp_cb_orig)
%     line = fgetl(fp_cb_orig);
%     %%disp(line);
%     cb_samples_orig(counter) = str2double(line);
%     counter = counter + 1;
%     if mod(counter, 500000) == 0
%         disp(counter)
%     end;
% end;
% fclose(fp_cb_orig);
% 
% counter = 1;
% fp_cr_orig = fopen('../../results/cr_samples_orig.txt');
% while ~feof(fp_cr_orig)
%     line = fgetl(fp_cr_orig);
%     %%disp(line);
%     cr_samples_orig(counter) = str2double(line);
%     counter = counter + 1;
%     if mod(counter, 500000) == 0
%         disp(counter)
%     end;
% end;
% fclose(fp_cr_orig);
% 
% save('y_samples_orig.mat', 'y_samples_orig');
% save('cb_samples_orig.mat', 'cb_samples_orig');
% save('cr_samples_orig.mat', 'cr_samples_orig');
% clear y_samples_orig;
% clear cb_samples_orig;
% clear cr_samples_orig;
% 
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% %%%%%%%%%%%%% Coefficients %%%%%%%%%%%%%%%
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% coeff_diffR = zeros(138943350, 1);
% coeff_diffC = zeros(138943350, 1);
% coeff_MuLE = zeros(138943350, 1);
coeff_orig = zeros(138943350, 1);
% 
% counter = 1;
% fp_coeff_diffR = fopen('../../results/coeff_diffR.txt');
% while ~feof(fp_coeff_diffR)
%     line = fgetl(fp_coeff_diffR);
%     %%disp(line);
%     coeff_diffR(counter) = str2double(line);
%     counter = counter + 1;
%     if mod(counter, 500000) == 0
%         disp(counter)
%     end;
% end;
% fclose(fp_coeff_diffR);
% 
% counter = 1;
% fp_coeff_diffC = fopen('../../results/coeff_diffC.txt');
% while ~feof(fp_coeff_diffC)
%     line = fgetl(fp_coeff_diffC);
%     %%disp(line);
%     coeff_diffC(counter) = str2double(line);
%     counter = counter + 1;
%     if mod(counter, 500000) == 0
%         disp(counter)
%     end;
% end;
% fclose(fp_coeff_diffC);
% 
% counter = 1;
% fp_coeff_MuLE = fopen('../../results/coeff_MuLE.txt');
% while ~feof(fp_coeff_MuLE)
%     line = fgetl(fp_coeff_MuLE);
%     %%disp(line);
%     coeff_MuLE(counter) = str2double(line);
%     counter = counter + 1;
%     if mod(counter, 500000) == 0
%         disp(counter)
%     end;
% end;
% fclose(fp_coeff_MuLE);

counter = 1;
fp_coeff_orig = fopen('../../results/coeff_orig.txt');
while ~feof(fp_coeff_orig)
    line = fgetl(fp_coeff_orig);
    %%disp(line);
    coeff_orig(counter) = str2double(line);
    counter = counter + 1;
    if mod(counter, 500000) == 0
        disp(counter)
    end;
end;
fclose(fp_coeff_orig);

% 
% save('coeff_diffR.mat', 'coeff_diffR');
% save('coeff_diffC.mat', 'coeff_diffC');
% save('coeff_MuLE.mat', 'coeff_MuLE');
save('coeff_orig.mat', 'coeff_orig');
% clear coeff_diffR;
% clear coeff_diffC;
% clear coeff_MuLE;
% 
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% %%%%%%%%%% Metrics calculation %%%%%%%%%%%
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% %%% Differential Raster %%%
% y_residues = load('y_residues_diffR.mat');
% cb_residues = load('cb_residues_diffR.mat');
% cr_residues = load('cr_residues_diffR.mat');
% en_y_diffR = entropy(y_residues.y_residues_diffR);
% en_cb_diffR = entropy(cb_residues.cb_residues_diffR);
% en_cr_diffR = entropy(cr_residues.cr_residues_diffR);
% 
% average_y_diffR = mean(y_residues.y_residues_diffR);
% average_cb_diffR = mean(cb_residues.cb_residues_diffR);
% average_cr_diffR = mean(cr_residues.cr_residues_diffR);
% 
% std_y_diffR = std(y_residues.y_residues_diffR);
% std_cb_diffR = std(cb_residues.cb_residues_diffR);
% std_cr_diffR = std(cr_residues.cr_residues_diffR);
% 
% max_y_diffR = max(y_residues.y_residues_diffR);
% max_cb_diffR = max(cb_residues.cb_residues_diffR);
% max_cr_diffR = max(cr_residues.cr_residues_diffR);
% min_y_diffR = min(y_residues.y_residues_diffR);
% min_cb_diffR = min(cb_residues.cb_residues_diffR);
% min_cr_diffR = min(cr_residues.cr_residues_diffR);
% 
% clear y_residues;
% clear cb_residues;
% clear cr_residues;
% 
% %%% Differential Central %%%
% y_residues = load('y_residues_diffC.mat');
% cb_residues = load('cb_residues_diffC.mat');
% cr_residues = load('cr_residues_diffC.mat');
% en_y_diffC = entropy(y_residues.y_residues_diffC);
% en_cb_diffC = entropy(cb_residues.cb_residues_diffC);
% en_cr_diffC = entropy(cr_residues.cr_residues_diffC);
% 
% average_y_diffC = mean(y_residues.y_residues_diffC);
% average_cb_diffC = mean(cb_residues.cb_residues_diffC);
% average_cr_diffC = mean(cr_residues.cr_residues_diffC);
% 
% std_y_diffC = std(y_residues.y_residues_diffC);
% std_cb_diffC = std(cb_residues.cb_residues_diffC);
% std_cr_diffC = std(cr_residues.cr_residues_diffC);
% 
% max_y_diffC = max(y_residues.y_residues_diffC);
% max_cb_diffC = max(cb_residues.cb_residues_diffC);
% max_cr_diffC = max(cr_residues.cr_residues_diffC);
% min_y_diffC = min(y_residues.y_residues_diffC);
% min_cb_diffC = min(cb_residues.cb_residues_diffC);
% min_cr_diffC = min(cr_residues.cr_residues_diffC);
% 
% clear y_residues;
% clear cb_residues;
% clear cr_residues;
% 
% %%% MuLE %%%
% y_samples = load('y_samples_MuLE.mat');
% cb_samples = load('cb_samples_MuLE.mat');
% cr_samples = load('cr_samples_MuLE.mat');
% en_y_MuLE = entropy(y_samples.y_samples_MuLE);
% en_cb_MuLE = entropy(cb_samples.cb_samples_MuLE);
% en_cr_MuLE = entropy(cr_samples.cr_samples_MuLE);
% 
% average_y_MuLE = mean(y_samples.y_samples_MuLE);
% average_cb_MuLE = mean(cb_samples.cb_samples_MuLE);
% average_cr_MuLE = mean(cr_samples.cr_samples_MuLE);
% 
% std_y_MuLE = std(y_samples.y_samples_MuLE);
% std_cb_MuLE = std(cb_samples.cb_samples_MuLE);
% std_cr_MuLE = std(cr_samples.cr_samples_MuLE);
% 
% max_y_MuLE = max(y_samples.y_samples_MuLE);
% max_cb_MuLE = max(cb_samples.cb_samples_MuLE);
% max_cr_MuLE = max(cr_samples.cr_samples_MuLE);
% min_y_MuLE = min(y_samples.y_samples_MuLE);
% min_cb_MuLE = min(cb_samples.cb_samples_MuLE);
% min_cr_MuLE = min(cr_samples.cr_samples_MuLE);
% 
% clear y_residues;
% clear cb_residues;
% clear cr_residues;
% 
% %%% orig %%%
% y_samples = load('y_samples_orig.mat');
% cb_samples = load('cb_samples_orig.mat');
% cr_samples = load('cr_samples_orig.mat');
% en_y_orig = entropy(y_samples.y_samples_orig);
% en_cb_orig = entropy(cb_samples.cb_samples_orig);
% en_cr_orig = entropy(cr_samples.cr_samples_orig);
% 
% average_y_orig = mean(y_samples.y_samples _orig);
% average_cb_orig = mean(cb_samples.cb_samples_orig);
% average_cr_orig = mean(cr_samples.cr_samples_orig);
% 
% std_y_orig = std(y_samples.y_samples_orig);
% std_cb_orig = std(cb_samples.cb_samples_orig);
% std_cr_orig = std(cr_samples.cr_samples_orig);
% 
% max_y_orig = max(y_samples.y_samples_orig);
% max_cb_orig = max(cb_samples.cb_samples_orig);
% max_cr_orig = max(cr_samples.cr_samples_orig);
% min_y_orig = min(y_samples.y_samples_orig);
% min_cb_orig = min(cb_samples.cb_samples_orig);
% min_cr_orig = min(cr_samples.cr_samples_orig);
% 
% clear y_residues;
% clear cb_residues;
% clear cr_residues;

%%%% Energy calculation %%%%
% coeff_diffR = load('coeff_diffR.mat');
% coeff_diffC = load('coeff_diffC.mat');
% coeff_MuLE = load('coeff_MuLE.mat');
% %coeff_orig = load('coeff_orig.mat');
% energy_diffR = sum((coeff_diffR.coeff_diffR.^2));
% energy_diffC = sum((coeff_diffC.coeff_diffC.^2));
% energy_MuLE = sum((coeff_MuLE.coeff_MuLE.^2));
% %energy_orig = sum((coeff_orig.coeff_orig.^2));

% clear coeff_diffR;
% clear coeff_diffC;
% clear coeff_MuLE;
% clear coeff_orig;