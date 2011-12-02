This is where the ogre Shared objects belong. If it they de not exist make sure that the dependencies are install which you can install with the following command (on ubuntu, Other distro users can atleast use this list to figure out what they need)
sudo apt-get install libfreetype6-dev libboost-date-time-dev libboost-thread-dev nvidia-cg-toolkit libfreeimage-dev zlib1g-dev libzzip-dev libois-dev libcppunit-dev doxygen libxt-dev libxaw7-dev libxxf86vm-dev libxrandr-dev libglu-dev cmake
1
Then you just Compile the project like normal and if ogre needs to be built then a copy will be built and installed into this folder.
