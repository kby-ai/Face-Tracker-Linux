
echo "*********************"   Get build started   "*********************"

unzip ./third_party/ncnn_x86_64/ncnn-20210720-ubuntu-2004-shared.zip

if [[ -d "build" ]]
then
    cd build
    
else
    mkdir build
    cd build
fi
cmake ..
make
cd ..
cp -r weights ./build
sudo cp -f ./ncnn-20210720-ubuntu-2004-shared/lib/libncnn.so* /usr/local/lib
sudo rm -r ./ncnn-20210720-ubuntu-2004-shared
echo "*********************"   Built successfully   "*********************"
exit


