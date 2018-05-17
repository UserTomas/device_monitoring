"use strict"

var os = require("os");
var notes = require('../controllers/note.controller.js');

const minVolume = 0, maxVolume = 360;

function Compass() {
  this.isOn = 1;
  this.firmwareVersion = '1.0.1';
  this.serialNumber = 'SN-' +this.randomSN();

  this.telemetry = {
    direction: 0
  };
  console.log("New compass created")
}

  //generate serial number for TV
  Compass.prototype.randomSN = function() {
      return Math.floor(Math.random() * 1000);
  }

  //upload tv telemetry to thingsboard
  Compass.prototype.updateTelemetry = function(){
      let timeS = 0;

      console.log("updateTelemetry");
      let col = notes.getCompassData();

      if (col) {
        this.telemetry.direction = this.calculateDirection(col);
        this.telemetry.isOn = 1;
        return this.telemetry;
    } else {
        let data = {};
        data.isOn = this.isOn;
        return data;
    }
  }

  Compass.prototype.calculateDirection = function(data) {
    if (data.hasOwnProperty('x')) {
      let heading = (Math.atan2(data.y, data.x)*180)/Math.PI;
      if (heading < 0) {
        heading += 360;
      }
      return heading;
    }else {
      return 0;
    }
  }

  //randomly genereate active channel on tv
  Array.prototype.randomChannel = function(previous) {
    let change = Math.random() < 0.7 ? 0 : 1;
    if (change) {
      return this[Math.floor(Math.random() * this.length)];
    }else {
      return previous;
    }
  }

  // Generates new random value that is within 3% range from previous value
  Compass.prototype.genNextValue = function(prevValue) {
      console.log("generate new values %d", prevValue);
      var value = prevValue + ((maxVolume - minVolume) * (Math.random() - 0.5)) * 0.9;
      value = Math.max(minVolume, Math.min(maxVolume, value));
      return Math.round(value * 10) / 10;
  }

module.exports = Compass;
