"use strict"

let Compass = require('./compass');


// Requires node.js and mqtt library installed.
var mqtt = require('mqtt');


const thingsboardHost = "demo.thingsboard.io";


// Default topics. See http://thingsboard.io/docs/reference/mqtt-api/ for more details.
const attributesTopic = 'v1/devices/me/attributes';
const telemetryTopic = 'v1/devices/me/telemetry';
const attributesRequestTopic = 'v1/devices/me/attributes/request/1';
const attributesResponseTopic = attributesRequestTopic.replace('request', 'response');


function ThingsBoard(accessToken) {

  console.log("ThingsBoard accessToken: %s", accessToken);

  // Initialization of mqtt client using Thingsboard host and device access token
  console.log('Connecting to: %s using access token: %s', thingsboardHost, accessToken);
  var client = mqtt.connect('mqtt://' + thingsboardHost, {username: accessToken});

  let cp = new Compass();

  var appState;
  // Telemetry upload is once per 5 seconds by default;
  var currentFrequency = 0.2;
  var uploadInterval;


  // Triggers when client is successfully connected to the Thingsboard server
  client.on('connect', function () {
    console.log('Client connected with accessToken: %s', accessToken);

    // Upload firmware version and serial number as device attribute using 'v1/devices/me/attributes' MQTT topic
    client.publish(attributesTopic, JSON.stringify({
      'firmwareVersion': cp.firmwareVersion,
      'serialNumber': cp.serialNumber,
      'runnningTime': Math.floor(Math.random() * 6) + 1
    }));
    // Subscribe to shared attribute changes on the server side
    client.subscribe(attributesTopic);
    // Publish request for 'appState' client attribute
    // and two shared attributes 'uploadFrequency' and 'latestVersion'
    client.publish(attributesRequestTopic, JSON.stringify({
      'clientKeys': 'appState',
      'sharedKeys': 'uploadFrequency,latestFirmwareVersion'
    }));

    // Schedule OS stats upload
    console.log('Uploading %s device telemetry with interval %s (sec)...',cp.serialNumber, currentFrequency);
    uploadInterval = setInterval(uploadStats, currentFrequency * 1000);
    // setInterval(updateTimeOn,1000);
  });

  client.on('message', function (topic, message) {
    if (topic === attributesTopic) {
      // Process attributes update notification
      console.log('Received attribute update notification: %s', message.toString());
      var data = JSON.parse(message);
      if (data.uploadFrequency && data.uploadFrequency != currentFrequency) {
        // Reschedule upload using new frequency
        rescheduleStatsUpload(data.uploadFrequency);
      }
      if (data.latestFirmwareVersion && data.latestFirmwareVersion != cp.firmwareVersion) {
        // Received new upload frequency configuration
        console.log('Device %s. New firmware version is available: %s',data.serialNumber, data.latestFirmwareVersion);
      }
    } else if (topic === attributesResponseTopic) {
      // Process response to attributes request
      console.log('Received response to attribute request: %s', message.toString());
      var data = JSON.parse(message);
      if (data.client && data.client.appState) {
        appState = data.client.appState;
        console.log('Restore app state to: %s', appState);
      } else {
        appState = random();
        console.log('This is first application launch. Going to publish random application state: %s', appState);
        client.publish(attributesTopic, JSON.stringify({'appState': appState}));
      }
      if (data.shared) {
        if (data.shared.uploadFrequency && data.shared.uploadFrequency != currentFrequency) {
          // Received new upload frequency configuration
          rescheduleStatsUpload(data.shared.uploadFrequency);
        }
        if (data.shared.latestFirmwareVersion && data.shared.latestFirmwareVersion != cp.firmwareVersion) {
          // Received new upload frequency configuration
          console.log('Device %s. New firmware version is available: %s',data.serialNumber, data.shared.latestFirmwareVersion);
        }
      }
    }
  })

  //generate random appstate
  function random() {
      return Math.floor(Math.random() * 1000);
  }

  // Reschedule of stats upload timer
  function rescheduleStatsUpload(uploadFrequency) {
    clearInterval(uploadInterval);
    currentFrequency = uploadFrequency;
    console.log('Device %s. Uploading OS stats with new interval %s (sec)...',cp.serialNumber, currentFrequency);
    uploadInterval = setInterval(uploadStats, currentFrequency * 1000);
  }

  // Upload OS stats using 'v1/devices/me/telemetry' MQTT topic
  function uploadStats() {
    let data = cp.updateTelemetry();
    console.log('Device %s. Publishing OS info & stats: %s',cp.serialNumber, JSON.stringify(data));
    client.publish(telemetryTopic, JSON.stringify(data));
  }

}

module.exports = ThingsBoard;
