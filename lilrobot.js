var hellobot = require('./hellobot');
var express = require('express');
var bodyParser = require('body-parser');

var app = express();
var port = process.env.PORT || 3000;

//body parser middleware
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: true}));

//test 
app.get('/', function (req, res) {res.status(200).send('Hello world!')});

//error handler
app.use(function (err, req, res, next){
	console.error(err.stack);
	res.status(400).send(err.message);
});

app.post('/hello', hellobot);

var request = require('request');
app.post('/sendmessage', function (req, res){
    console.log("hello dudddeee");
    console.log("data is " + req.body.data);
    var spec_req = req.body.data;
    if(spec_req == "move"){
        request.post({
        url: 'https://hooks.slack.com/services/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',
        //form: {payload: JSON.stringify({text: 'meow'})}
        form: {payload: JSON.stringify({text: 'http://media2.giphy.com/media/XlFfSD0CiyGLC%2F200.gif'})}
        //form: {payload: JSON.stringify({text: 'no one move!'})}
    }, function(err,httpResponse,body) {
        console.log("back", body);
    });
    }
    else if (spec_req=="laugh"){
         request.post({
        url: 'https://hooks.slack.com/services/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',
        form: {payload: JSON.stringify({text: 'http://i.giphy.com/ZDKZ8hiISoEy4.gif'})}
    }, function(err,httpResponse,body) {
        console.log("back", body);
    });
    }
    
    else{
    request.post({
        url: 'https://hooks.slack.com/services/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',
        //form: {payload: JSON.stringify({text: 'meow'})}
        //form: {payload: JSON.stringify({text: 'http://media2.giphy.com/media/XlFfSD0CiyGLC%2F200.gif'})}
        form: {payload: JSON.stringify({command:'/giphy', text:'waves'})}
            //text: 'http://media2.giphy.com/media/XlFfSD0CiyGLC%2F200.gif'})}
    }, function(err,httpResponse,body) {
        console.log("back", body);
    });
        }

    res.status(200).send('Sent!');
});

app.listen(port, function(){
	console.log('Slack bot listening on port ' + port);
});