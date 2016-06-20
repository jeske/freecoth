#!/bin/sh

# Automatically generate all of the images for the screenshots section of the
# Freecloth webpage. See autogengroup.sh for required software.

./autogengroup.sh nbPatches "011 022 033 066 099 132"
./autogengroup.sh constraint "table_circle table_square centre corners4 corners3a corners3b" -maxTimestep 0
# tighter stretch limit required for this one.
./autogengroup.sh stretch "0020 0050 0100 0200 1000"
./autogengroup.sh shear "01 05 10 20 50"
./autogengroup.sh bend "0.001 0.005 0.010 0.020 0.100"
# Run damp tests longer, to allow oscillations to die out
./autogengroup.sh stretchDamp "010 020 040 100" -batch 4
./autogengroup.sh shearDamp "00.02 00.20 01.00 02.00 04.00" -batch 4
./autogengroup.sh bendDamp "0.0002 0.0010 0.0020 0.0040 0.0100" -batch 4
./autogengroup.sh density "0.01 0.05 0.10 0.20 1.00"
./autogengroup.sh drag "0.01 0.05 0.10 0.20 1.00"
./autogengroup.sh stretchLimit "5e-6 1e-5 2e-5 5e-5 1e-4"
./autogengroup.sh tolerance "0.00001 0.00010 0.00100 0.01000 0.10000"
./autogengroup.sh maxTimestep "0.001 0.002 0.005 0.010 0.020"
