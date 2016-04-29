#!/bin/bash

awk '$3<=6000 && $3 >= 4000{print NR, $3, $4, $1}' datafile.out > formate_order.data1
awk '$3>=6000 && $3<=8000{print NR, $3, $4, $1}' datafile.out > formate_order.data2
