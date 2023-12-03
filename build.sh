# if [ $1 = "debug" ];
FLAGS="DEBUG"
if [ $FLAGS = "DEBUG" ];
then
    if [ ! -d "build" ];
    then 
    mkdir build 
    fi 
cd build 
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
make 
./elliptic_curve 
cd ..
#TODO
fi
