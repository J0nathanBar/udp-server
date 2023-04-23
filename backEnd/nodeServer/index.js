const express = require("express");
const bodyParser = require('body-parser');
const cors = require("cors");
const app = express();
const fs = require('fs')
const WebSocket = require('ws')
const { spawn } = require('child_process')
app.use(cors());
app.use(bodyParser.json());
let udpTx = null;
const port = process.env.PORT || "5000";




app.post("/Transmitter", ({ body }, res) => {
  console.log(body.a)
  res.json(body)
  console.log("file written")
  if (udpTx == null) {
    udpTx = udpTx = spawn('../UdpTx/build/UdpTx', [], {
      stdio: ['pipe', 'inherit', 'inherit'],
    });
    console.log("udpTx process started");
  }
  else console.log("already exists")
  if (udpTx != null) {
    udpTx.on('close', (code) => {
      console.log(`C++ process exited with code ${code}`)
      udpTx = null
    });
  }
  if (udpTx != null) {
    udpTx.stdin.write(JSON.stringify(body.a.attributes) + '\n')
  }
  if (udpTx.stdout) {
    udpTx.stdout.on('data', (data) => {
      console.log(`C++ process output: ${data}`)
    });
  }


});
app.post("/Receiver", ({ body }, res) => {

  console.log(body.a)
  res.json(body)
  fs.writeFile(`/home/jonny/Desktop/project/udp-server/backEnd/nodeServer/RecvConf.json`, JSON.stringify(body.a.attributes), err => {
    if (err) {
      console.error(err);
    }
    console.log("file written")
  })


});
app.listen(port, () => { console.log("Listening on port " + port) })


if (udpTx != null) {

  udpTx.stdout.on('data', (data) => {
    console.log(`C++ process output: ${data}`);
  });
}


process.on('SIGINT', () => {
  console.log('Received SIGINT signal, terminating C++ process...');

  // Send a SIGINT signal to the child process to terminate it
  if (udpTx != null) {
    udpTx.kill('SIGINT');
    console.log("killed UdpTx")
  }

  // Exit the Node.js process
  process.exit();
});



