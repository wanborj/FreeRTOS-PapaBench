#!/bin/bash

awk 'NR<=200{print NR, $1}' datafile.out > formate_order.data1
awk 'NR>200 && NR<=400{print NR, $1}' datafile.out > formate_order.data2
