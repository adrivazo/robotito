module.exports = function (req, res, next) {
  var userName = req.body.user_name;
  var command = req.body.data;
    log("let's see if there was a command");
    log(command);
  var botPayload = {
    text : 'Hi, ' + userName + '!'
  };

  // avoid infinite loop
  if (userName !== 'slackbot') {
    return res.status(200).json(botPayload);
  } else {
    return res.status(200).end();
  }
}
