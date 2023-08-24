
echo "*********************"   Getting Library Installed   "*********************"
echo "Vulkan Installing..."
wget -qO - http://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-focal.list http://packages.lunarg.com/vulkan/lunarg-vulkan-focal.list
sudo apt update
sudo apt install vulkan-sdk
if [ $? -eq 0 ]; then
   echo 8:  "vulkan-sdk was installed successfully"
else
   echo "vulkan-sdk installing failed!"
   echo "please make sure if internet connection is bad or not"
   exit
fi    

echo "*********************"   Library Installed Successfully   "*********************"
exit
