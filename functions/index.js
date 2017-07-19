const functions = require('firebase-functions');
const mqtt = require('mqtt');
const url = require('url');

const device = functions.config().mqtt.device;
const options = {
  port: 15546,
  clientId: 'mqttjs_' + Math.random().toString(16).substr(2, 8),
  username: functions.config().mqtt.user,
  password: functions.config().mqtt.password,
};

exports.publish = functions.https.onRequest((request, response) => {

  const { branch, state } = JSON.parse(request.body.payload);
  if (branch !== 'master') response.status(405).send(state);

  const client = mqtt.connect('mqtt://m20.cloudmqtt.com', options);

  client.on('connect', function() {
    client.publish(`devices/${ device }/light/state/set`, state, { retain: true }, function() {
      console.log(`published;device:${ device };state:${ state }`);
      client.end();
      response.status(200).send(state);
    });
  });
});
