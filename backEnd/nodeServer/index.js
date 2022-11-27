const express = require("express");
const bodyParser = require('body-parser');
const cors = require("cors");
const app = express();
const fs = require('fs')
var cp = require('child_process')
app.use(cors());
// parse application/json
app.use(bodyParser.json());
const port = process.env.PORT || "5000";



app.post("/api", (req, res) => {
    //res.status(200).send("WHATABYTE: Food For Devs");
    console.log(req.body)
    res.json(req.body)
    fs.writeFile("/home/jonathan/Desktop/project/backEnd/nodeServer/message.json", JSON.stringify(req.body), err=> {
        if(err){
            console.error(err);
        }
    })
})
app.listen(port, () => { console.log("Listening on port " + port) })