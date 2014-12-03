#! /bin/bash
echo 'Scenario A'
perl analyze.pl data/A_MA  | tee A.txt

echo 'Scenario B'
echo 'Link 1'
perl analyze.pl data/B_S0_VM2_MA | tee B.1.txt
echo 'Link 2'
perl analyze.pl data/B_S1_VM2_MA | tee B.2.txt
echo 'Link 3'
perl analyze.pl data/B_S2_VM4_MA | tee B.3.txt
echo 'Link 4'
perl analyze.pl data/B_S3_VM4_MA | tee B.4.txt
echo 'Link 5'
perl analyze.pl data/B_VM2_VM3_MA | tee B.5.txt
echo 'Link 6'
perl analyze.pl data/B_VM4_VM3_MA | tee B.6.txt
echo 'Link 7'
perl analyze.pl data/B_VM3_C_MA | tee B.7.txt

echo 'Scenario C'
echo 'Link 1'
perl analyze.pl data/C_S0_VM2_MA | tee C.1.txt
echo 'Link 2'
perl analyze.pl data/C_S1_VM2_MA | tee C.2.txt
echo 'Link 3'
perl analyze.pl data/C_S2_VM4_MA | tee C.3.txt
echo 'Link 4'
perl analyze.pl data/C_S3_VM4_MA | tee C.4.txt

echo 'Scenario D'
echo 'Link 1'
perl analyze.pl data/D_S0_VM2_MA | tee D.1.txt
echo 'Link 2'
perl analyze.pl data/D_S1_VM2_MA | tee D.2.txt
echo 'Link 3'
perl analyze.pl data/D_S2_VM4_MA | tee D.3.txt
echo 'Link 4'
perl analyze.pl data/D_S3_VM4_MA | tee D.4.txt
