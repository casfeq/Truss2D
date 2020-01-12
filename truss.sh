clear
clear
clear

export sourceName="trussSolve"

# COMPILE
cd build
cmake ..
make
cd ..
echo ""

# RUN
rm -rf export/*
rm -rf plot/*
cd build
echo "-- Solving truss problem"
./$sourceName
cd ..
echo ""
echo "-- Plotting results"
python3 -W ignore ./postpro/plotSolution.py
echo ""
# rm -rf export/*
