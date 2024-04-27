var express = require('express');
var http = require('http');

var app = express();

app.use(express.urlencoded({ extended: true }));
app.use(express.static('public'));

app.post('/login', function (req, res) {
    console.log("Username: " + req.body.username);
    console.log("Password: " + req.body.password);
    res.send("OK!");
});

var httpServer = http.createServer(app);

httpServer.listen(3000, function () {
  console.log('Example app listening on port 3000!');
});

