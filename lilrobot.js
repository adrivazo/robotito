var hellobot = require('./hellobot');
var express = require('express');
var bodyParser = require('body-parser');

var app = express();
var port = process.env.PORT || 3000;

//body parser middleware
app.use(bodyParser.urlencoded({extended: true}));

//test 
app.get('/', function (req, res) {res.status(200).send('Hello world!')});

//error handler
app.use(function (err, req, res, next){
	console.error(err.stack);
	res.status(400).send(err.message);
});

app.post('/hello', hellobot);

app.get('/sendmessage', function (){
    var request = require('request');
    request({
        url:'https://hooks.slack.com/services/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',
        method:"POST",
        json:true,
        headers:{
        "content-type":"application/json"},
        body: JSON.stringify(
     {        payload: {
                    text:"hello world!!"
                }
            })
        ,
        function (error, response, body){
            console.log(response);
        }
    );
});

app.listen(port, function(){
	console.log('Slack bot listening on port ' + port);
});