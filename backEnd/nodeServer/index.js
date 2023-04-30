const express = require("express");
const bodyParser = require('body-parser');
const cors = require("cors");
const app = express();
const { spawn } = require('child_process')
const mysql = require('mysql2');
app.use(cors())
app.use(bodyParser.json())
let udpTx = null
let udpRx = null
const port = process.env.PORT || "5000";




function isTX() {
  return udpTx !== null
}
function isRX() {
  return udpRx !== null
}

app.post("/stopTX", ({ body }, res) => {
  if (udpTx) {
    udpTx.kill()
    udpTx = null
    console.log("TX killed ")
  }
})
app.post("/stopRX", ({ body }, res) => {
  if (udpRx) {
    udpRx.kill()
    udpRx = null
    console.log("RX killed ")
  }
})

app.get('/isTxRunning', (req, res) => {
  const response = {
    isTx: isTX()
  };
  res.json(response);
});

app.get('/isRxRunning', (req, res) => {
  const response = {
    isRx: isRX()
  };
  res.json(response);
});

app.post("/Transmitter", ({ body }, res) => {
  res.json(body)
  if (udpTx == null) {
    udpTx = udpTx = spawn('../UdpTx/build/UdpTx', [], {
      stdio: ['pipe', 'inherit', 'inherit'],
    });
    console.log("udpTx process started");
  }
  if (udpTx !== null) {
    udpTx.on('close', (code) => {
      console.log(`Tx process exited with code ${code}`)
      udpTx = null
    });
  }
  if (udpTx !== null) {
    udpTx.stdin.write(JSON.stringify(body.a.attributes) + '\n')
  }
  if (udpTx.stdout) {
    udpTx.stdout.on('data', (data) => {
      console.log(`Tx process output: ${data}`)
    });
  }


});
app.post("/Receiver", ({ body }, res) => {

  console.log(body.a)
  res.json(body)

  if (udpRx == null) {
    udpRx = udpRx = spawn('../UdpRx/build/UdpRx', [], {
      stdio: ['pipe', 'pipe', 'pipe'],
    });
    console.log("udpRx process started");
  }
  if (udpRx !== null) {
    udpRx.on('close', (code) => {
      console.log(`Rx process exited with code ${code}`)
      udpRx = null
    });
  }
  if (udpRx !== null) {
    udpRx.stdin.write(JSON.stringify(body.a.attributes) + '\n')
  }
  if (udpRx.stdout) {
    udpRx.stdout.on('data', (data) => {
      const output = data.toString()
      console.log(`Rx process output: ${data}`)
      // if (output.includes('egg'))
      //   console.log("eggy weggy leggy man!")

    });
  }


});


app.get('/Statistics', (req, res) => {
  const connection = mysql.createConnection({
    host: 'localhost',
    user: 'broman',
    password: 'yeet',
    database: 'my_database'
  });
  connection.connect(function(err) {
    if (err) {
      console.error('Error connecting to database: ' + err.stack);
      return;
    }
    console.log('Connected to database.');
  });
  connection.query('SELECT first_detected_time,saved_time,block_size FROM TX_TABLE JOIN RX_TABLE ON TX_TABLE.file_id = RX_TABLE.file_id', function(err, results, fields) {
    if (err) connection.end();
    console.log(results);
    res.json(results)
    
  });
  connection.end();
 
});
app.listen(port, () => { console.log("Listening on port " + port) })





process.on('SIGINT', () => {
  console.log('Received SIGINT signal, terminating C++ process...');
  if (udpTx != null) {
    udpTx.kill('SIGINT');
    console.log("killed UdpTx")
  }
  if (udpRx != null) {
    udpRx.kill('SIGINT');
    console.log("killed UdpRx")
  }
  process.exit();
});



