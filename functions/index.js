const functions = require('firebase-functions');
const fetch = require('node-fetch');
const mqtt = require('mqtt');
const url = require('url');

const { device, user, password } = functions.config();

const options = {
  port: 15546,
  clientId: 'mqttjs_' + Math.random().toString(16).substr(2, 8),
  username: user,
  password,
};

exports.publish = functions.https.onRequest((request, response) => {

const { payload } = request.body;
const { branch, state } = JSON.parse(payload);
if (branch !== 'master') response.status(405).send(state);

fetch('https://api.travis-ci.org/config')
  .then((res) => res.json())
  .then(function(json) {
    const publicKey = config.notifications.webhook.public_key;
    const signature = request.get('Signature');
    const decodedSignature = Buffer.from(signature, 'base64');


    const client = mqtt.connect('mqtt://m20.cloudmqtt.com', options);

    client.on('connect', function() {
      client.publish(`devices/${ device }/light/state/set`, state, { retain: true }, function() {
        console.log(`published;device:${ device };state:${ state }`);
        client.end();
        response.status(200).send(state);
      });
    });
  });
});
