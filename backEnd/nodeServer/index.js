/*const express = require("express");
const bodyParser = require('body-parser');
const cors = require("cors");
const app = express();
const fs = require('fs')
var cp = require('child_process')
app.use(cors());
// parse application/json
app.use(bodyParser.json());
const port = process.env.PORT || "5000";



app.post("/api", ({body}, res) => {
    const {filename, content} = body.a.attributes
    console.log("filename: "+filename +" content: " +content)
    console.log(body.a)
    res.json(body)
    fs.writeFile(`/home/jonny/Desktop/project/udp-server/backEnd/nodeServer/${filename}`,JSON.stringify(content), err=> {   

        if(err){
            console.error(err);
        }
        console.log("file written")
    })
})
app.listen(port, () => { console.log("Listening on port " + port) })*/


const express = require('express');
const multer = require('multer');
const path = require('path');

// Set up the express app
const app = express();

// Set up the multer middleware
const upload = multer({
  storage: multer.diskStorage({
    destination: (req, file, cb) => {
      cb(null, './FoldersToSend');
    },
    filename: (req, file, cb) => {
      cb(null, `${file.fieldname}-${Date.now()}${path.extname(file.originalname)}`);
    },
  }),
});

// Set up the file upload route
app.post('/upload', upload.single('folder'), (req, res) => {
  // The uploaded folder is available in req.file
  console.log(req.file);

  res.sendStatus(200);
});

// Start the server
app.listen(5000, () => {
  console.log('Server listening on port 5000');
});

