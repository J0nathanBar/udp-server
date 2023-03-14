echo installing node and react
cd --
sudo curl -sL https://deb.nodesource.com/setup_14.x — Node.js 14 LTS "Fermium"  | sudo -E bash -
sudo apt install nodejs
sudo apt install npm
sudo npm install -g create-react-app
sudo npm install -g node-modules
cd -
cd ../frontEnd/src/
sudo npm install
cd -
./installNodeDemon.sh
