// node serverRun

const express = require("express");
const bodyParser = require("body-parser");
const app = express();
const fs = require('fs');

app.use(bodyParser.urlencoded({ extended: true }));

app.get('/esp/restart', (req, res) => {
    console.log('GET /esp/restart');
    res.set('Access-Control-Allow-Origin', '*');
    res.send({"state": "OK"});
});

app.post("/esp/saveConfig", (req, res) => {
    console.log('POST /esp/saveConfig');
    res.set('Access-Control-Allow-Origin', '*');
    let data = req.body.config;
    setTimeout(() => {
        res.send({"state": "OK"});
        if(data) fs.writeFileSync('../public/config.json', data);
    }, 2000);
    
});

app.get('/esp/bright', (req, res) => {
    console.log('GET /esp/bright');
    res.set('Access-Control-Allow-Origin', '*');
    res.send({"state": "OK"});
});

app.get('/esp/sensitivity', (req, res) => {
    console.log('GET /esp/sensitivity');
    res.set('Access-Control-Allow-Origin', '*');
    res.send({"state": "OK"});
});

app.get('/esp/dispToggle', (req, res) => {
    console.log('GET /esp/dispToggle');
    res.set('Access-Control-Allow-Origin', '*');
    res.send({"state": "OK"});
});

app.get('/esp/brightLimit', (req, res) => {
    console.log('GET /esp/brightLimit');
    res.set('Access-Control-Allow-Origin', '*');
    res.send({"state": "OK"});
});

app.get('/esp/color', (req, res) => {
    console.log('GET /esp/color');
    res.set('Access-Control-Allow-Origin', '*');
    res.send({"state": "OK"});
});

app.get('/esp/syncClock', (req, res) => {
    console.log('GET /esp/syncClock');
    res.set('Access-Control-Allow-Origin', '*');
    res.send({"state": "OK"});
});

app.get('/esp/syncdialog', (req, res) => {
    console.log('GET /esp/syncDialog');
    res.set('Access-Control-Allow-Origin', '*');
    res.send("22:35");
});

app.post('/esp/changePass', (req, res) => {
    console.log('GET /esp/changePass');
    res.set('Access-Control-Allow-Origin', '*');
    res.send("OK");
});

app.post('/esp/fileUpload', (req, res) => {
    console.log('POST /esp/fileUpload');
    res.set('Access-Control-Allow-Origin', '*');
    res.send("OK");
});

app.post('/esp/rename', (req, res) => {
    console.log('POST /esp/rename');
    res.set('Access-Control-Allow-Origin', '*');
    res.send("OK");
});

app.post('/esp/delete', (req, res) => {
    console.log('POST /esp/delete');
    res.set('Access-Control-Allow-Origin', '*');
    res.send("OK");
});

app.post('/esp/fwUpdate', (req, res) => {
    console.log('POST /esp/fwUpdate');
    res.set('Access-Control-Allow-Origin', '*');
    res.send("OK");
});

app.get('/esp/login', (req, res) => {
    console.log('GET /esp/login');
    res.set('Access-Control-Allow-Origin', '*');
    res.send("OK");
    //res.send("error");
});

app.get('/esp/volume', (req, res) => {
    console.log('GET /esp/volume');
    res.set('Access-Control-Allow-Origin', '*');
    res.send({"state": "OK"});
});

app.get('/esp/equalizer', (req, res) => {
    console.log('GET /esp/equalizer');
    res.set('Access-Control-Allow-Origin', '*');
    res.send({"state": "OK"});
});

app.get('/esp/mp3play', (req, res) => {
    console.log('GET /esp/mp3play');
    res.set('Access-Control-Allow-Origin', '*');
    res.send({"state": "OK"});
});

app.get('/esp/mp3stop', (req, res) => {
    console.log('GET /esp/mp3stop');
    res.set('Access-Control-Allow-Origin', '*');
    res.send({"state": "OK"});
});

app.get('/esp/animation', (req, res) => {
    console.log('GET /esp/animation');
    res.set('Access-Control-Allow-Origin', '*');
    res.send({"state": "OK"});
});

app.listen(80, () => {
    console.log('listening on port 80')
})