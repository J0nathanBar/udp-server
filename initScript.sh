echo hello-world
cd frontEnd/src/
npm start &
echo react-running
cd -
cd backEnd/nodeServer/
npm run dev &
echo nodeJS-running
cd -
cd backEnd/UdpRx/build/
make
./UdpRx
echo Rx-running

