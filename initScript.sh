sudo iptables -A INPUT -p tcp --dport 42069 -j DROP
sudo iptables -A INPUT -p udp --dport 42069 -j DROP
echo hello-world
cd frontEnd/src/
npm start &
echo react-running
cd -
cd backEnd/nodeServer/
npm run dev 
echo nodeJS-running

