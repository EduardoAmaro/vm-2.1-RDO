solution_list = {'diffCDC'};
lambda_list = {'322_1_30_8'};
ppm_list = {'000', '001' '002', '003', '004', '005', '006', '007', '008', '009', '010', '011', '012'};
Y_PNSR_SUM = 0;
YUV_PNSR_SUM = 0;
Y_SSIM_SUM = 0;
Y_PSNR_list = zeros(13, 13);
Y_SSIM_list = zeros(13, 13);
YUV_PSNR_list = zeros(13, 13);
counter_Y_PSNR = 0;
counter_YUV_PSNR = 0;
counter_Y_SSIM = 0;

for i = 1:numel(solution_list)
    for j = 1:numel(lambda_list)
        for l = 1:numel(ppm_list)
            for k = 1:numel(ppm_list)
                arq1 = ['/home/douglascorrea/light-field/dataset/Lenslet/Bikes/', ppm_list{k}, '_', ppm_list{l},'.ppm'];
                arq2 = ['/home/douglascorrea/light-field/resultados_prediction/Bikes/decoding/bitplanes/', solution_list{i}, '_', lambda_list{j}, '/', ppm_list{k}, '_', ppm_list{l},'.ppm'];
                disp(arq1);
                disp(arq2);
                im1=imread(arq1);
                im2=imread(arq2);

                ref=bitshift(im1,-6); rec=bitshift(im2,-6);

                [Y_PSNR YUV_PSNR Y_SSIM] = QM(ref,rec,10,10)
                
                Y_PSNR_list(k, l) = Y_PSNR;
                YUV_PSNR_list(k, l) = YUV_PSNR;
                Y_SSIM_list(k, l) = Y_SSIM;
                if(Y_PSNR < 70)
                    counter_Y_PSNR = counter_Y_PSNR + 1;
                    Y_PNSR_SUM = Y_PNSR_SUM + Y_PSNR;
                end;
                if(YUV_PSNR < 70)
                    counter_YUV_PSNR = counter_YUV_PSNR + 1;
                    YUV_PNSR_SUM = YUV_PNSR_SUM + YUV_PSNR;
                end;
                if(Y_SSIM < 1)
                    counter_Y_SSIM = counter_Y_SSIM + 1;
                    Y_SSIM_SUM = Y_SSIM_SUM + Y_SSIM;
                end;             
            end;
        end;
    end;
end;

AVERAGE_Y_PNSR = Y_PNSR_SUM/counter_Y_PSNR;
AVERAGE_YUV_PNSR =  YUV_PNSR_SUM/counter_YUV_PSNR;
AVERAGE_Y_SSIM =  Y_SSIM_SUM/counter_Y_SSIM;
