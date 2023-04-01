echo installing node and react
cd --
sudo curl -sL https://deb.nodesource.com/setup_14.x — Node.js 14 LTS "Fermium"  | sudo -E bash -
sudo apt install nodejs
sudo apt install npm
sudo npm install -g create-react-app
sudo npm install -g node-modules
cd -
cd ../frontEnd/
sudo npm install
sudo npm install @mui/joy @emotion/react @emotion/styled
sudo npm i react-router-dom

cd -
./installNodeDemon.sh
