#!/bin/bash

################################################################################

TOOL=./bin/aero_coefficients

################################################################################

rm *.csv

################################################################################

# wing - positive
$TOOL \
./data/c172/aoa_cx.csv c172_pos_cx.csv \
./data/c172/aoa_cz.csv c172_pos_cz.csv \
14.9  1.42 \
21.0  0.90 \
45.0  1.10 \
      0.027 \
10.0  0.05872 \
15.0  0.09924 \
19.0  0.2 \
60.0  1.6 \
      2.1

# wing - negative
$TOOL \
./data/c172/aoa_cx.csv c172_neg_cx.csv \
./data/c172/aoa_cz.csv c172_neg_cz.csv \
14.9  1.09 \
21.0  0.71 \
45.0  1.10 \
      0.027 \
10.0  0.05872 \
15.0  0.09924 \
19.0  0.2 \
60.0  1.6 \
      2.1
      
################################################################################

# horizontal stabilizer - positive
$TOOL \
./data/c172/aoa_stab_cx.csv c172_stab_h_pos_cx.csv \
./data/c172/aoa_stab_cz.csv c172_stab_h_pos_cz.csv \
28.0  1.75 \
48.0  1.35 \
60.0  1.55 \
      0.03051 \
14.0  0.12984 \
30.0  0.64294 \
50.0  1.23 \
70.0  1.94 \
      2.6

# horizontal stabilizer - negative    
$TOOL \
./data/c172/aoa_stab_cx.csv c172_stab_h_neg_cx.csv \
./data/c172/aoa_stab_cz.csv c172_stab_h_neg_cz.csv \
24.0  1.45 \
27.0  0.96 \
53.0  1.52 \
      0.03051 \
14.0  0.12984 \
30.0  0.64294 \
50.0  1.23 \
70.0  1.94 \
      2.6

################################################################################

# horizontal stabilizer - positive HD
$TOOL \
./data/c172/aoa_stab_hd_cx.csv c172_stab_h_pos_cx_hd.csv \
./data/c172/aoa_stab_hd_cz.csv c172_stab_h_pos_cz_hd.csv \
28.0  1.75 \
48.0  1.35 \
60.0  1.55 \
      0.03051 \
14.0  0.12984 \
30.0  0.64294 \
50.0  1.23 \
70.0  1.94 \
      2.6

# horizontal stabilizer - negative HD  
$TOOL \
./data/c172/aoa_stab_hd_cx.csv c172_stab_h_neg_cx_hd.csv \
./data/c172/aoa_stab_hd_cz.csv c172_stab_h_neg_cz_hd.csv \
24.0  1.45 \
27.0  0.96 \
53.0  1.52 \
      0.03051 \
14.0  0.12984 \
30.0  0.64294 \
50.0  1.23 \
70.0  1.94 \
      2.6
      
################################################################################

# vertical stabilizer
$TOOL \
./data/c172/aoa_stab_cx.csv c172_stab_v_cx.csv \
./data/c172/aoa_stab_cy.csv c172_stab_v_cy.csv \
10.0  -0.72 \
20.0  -1.23 \
60.0  -1.84 \
      0.03739 \
10.0  0.14962 \
20.0  0.55510 \
30.0  0.95828 \
60.0  2.31193 \
      2.57282
      
# vertical stabilizer - HD 
$TOOL \
./data/c172/aoa_stab_hd_cx.csv c172_stab_v_cx_hd.csv \
./data/c172/aoa_stab_hd_cz.csv c172_stab_v_cy_hd.csv \
10.0  -0.72 \
20.0  -1.23 \
60.0  -1.84 \
      0.03739 \
10.0  0.14962 \
20.0  0.55510 \
30.0  0.95828 \
60.0  2.31193 \
      2.57282

################################################################################

# default values
# $TOOL \
# ./data/c172/aoa_cx.csv c172_cx.csv \
# ./data/c172/aoa_cz.csv c172_cz.csv \
# 15.0  1.2 \
# 20.0  0.8 \
# 45.0  1.0 \
#       0.01 \
# 10.0  0.1 \
# 15.0  0.2 \
# 20.0  0.3 \
# 60.0  1.5 \
#       2.1

################################################################################
