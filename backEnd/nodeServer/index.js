const express = require("express");
const bodyParser = require('body-parser');
const cors = require("cors");
const app = express();
const fs = require('fs')
var cp = require('child_process')
const WebSocket = require('ws')
app.use(cors());
// parse application/json
app.use(bodyParser.json());
const port = process.env.PORT || "5000";



// app.post("/api", ({ body }, res) => {
//     const { filename, content } = body.a.attributes
//     console.log("filename: " + filename + " content: " + content)
//     console.log(body.a)
//     res.json(body)
//     fs.writeFile(`/home/jonny/Desktop/project/udp-server/backEnd/nodeServer/${filename}`, JSON.stringify(content), err => {

//         if (err) {
//             console.error(err);
//         }
//         console.log("file written")
//     })
// })
// app.listen(port, () => { console.log("Listening on port " + port) })

app.post("/Transmitter", ({ body }, res) => {
  //  const { srcPath, filePacket, blockSize } = body.a.attributes

  console.log(body.a)
  res.json(body)
  fs.writeFile(`/home/jonny/Desktop/project/udp-server/backEnd/nodeServer/TransConf.json`, JSON.stringify(body.a.attributes), err => {
    if (err) {
      console.error(err);
    }
    console.log("file written")
  })
});
app.post("/Receiver", ({ body }, res) => {
  //  const { srcPath, filePacket, blockSize } = body.a.attributes

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




