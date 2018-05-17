var mongoose = require('mongoose');

var NoteSchema = mongoose.Schema({
    // title: String,
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

module.exports = mongoose.model('Note', NoteSchema);
