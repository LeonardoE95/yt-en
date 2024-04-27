var fs = require('fs');
var http = require('http');
var https = require('https');
var express = require('express');

var app = express();

app.use(express.urlencoded({ extended: true }));
app.use(express.static('public'));

app.post('/login', function (req, res) {
    console.log("Username: " + req.body.username);
    console.log("Password: " + req.body.password);
    res.send("OK!");
});

var httpServer = http.createServer(app);

// THESE ARE NOT TRUSTED: RED

var privateKey  = fs.readFileSync('tls/not-trusted-server.key', 'utf8');
var certificate = fs.readFileSync('tls/not-trusted-server.crt', 'utf8');

// THESE ARE TRUSTED: GREEN
// 
// var privateKey  = fs.readFileSync('tls/localhost-key.pem', 'utf8');
// var certificate = fs.readFileSync('tls/localhost.pem', 'utf8');

var credentials = { key: privateKey , cert: certificate };

var httpsServer = https.createServer(credentials, app);

httpsServer.listen(3000, function () {
  console.log('Example app listening on port 3000!');
});
