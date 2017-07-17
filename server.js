const mqtt = require('mqtt');
const url = require('url');

const options = {
  port: 15546,
  clientId: 'mqttjs_' + Math.random().toString(16).substr(2, 8),
  username: 'dingbot',
  password: 'D1nGB0T',
};

// Create a client connection
const client = mqtt.connect('mqtt://m20.cloudmqtt.com', options);

client.on('connect', function() { // When connected

  // subscribe to a topic
  client.subscribe('devices/', function() {
    // when a message arrives, do something with it
    client.on('message', function(topic, message, packet) {
      console.log("Received '" + message + "' on '" + topic + "'");
    });
  });

  // publish a message to a topic
  client.publish('devices/12321', 'Hello1', function() {
    console.log("Message is test");
    client.end(); // Close the connection when published
  });

  // client.publish('devices/123532', 'Hello id', function() {
  //   console.log("Message is test");
  //   client.end(); // Close the connection when published
  // });

});
