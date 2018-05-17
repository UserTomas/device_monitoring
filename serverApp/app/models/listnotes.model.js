var mongoose = require('mongoose');
var Promise = require("bluebird");


var NoteSchema = mongoose.Schema({
    acc: {
      aX: Number,
      aY: Number,
      aZ: Number
    },
    mag: {
      mX: Number,
      mY: Number,
      mZ: Number
    },
    timestamp: Number,
}, {
    timestamps: true
});

var NoteListSchema = mongoose.Schema({
    title: String,
    timestamp: Number,
    children: [NoteSchema]
}, {
    timestamps: true,
    collections: 'user1data'
});

module.exports = mongoose.model('NoteList', NoteListSchema);
