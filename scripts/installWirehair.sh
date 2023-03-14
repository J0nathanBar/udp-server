echo install-WireHair
cd ../backEnd/wirehair/wirehair
gh repo clone catid/wirehair
mkdir build
cd build
cmake ..
make
sudo make install
