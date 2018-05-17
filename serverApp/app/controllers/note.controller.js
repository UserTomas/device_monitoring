var Note = require('../models/note.model.js');
let NoteList = require('../models/listnotes.model.js');

let ThingsBoard = require('../thingsBoard/thingsBoard.js');

let compassData = {};
//thingsBorad client connection
let tb = new ThingsBoard("<yourAccessToken>");

//create one new record
exports.create = function(req, res) {
    // Create and Save a new Note
    console.log(req.body);
    if(!req.body.acc) {
        return res.status(400).send({message: "Note can not be empty"});
    }
    console.log(req.body);
    var note = new Note({title: req.body.title || "Untitled Note", acc: req.body.acc,
                        mag: req.body.mag, timestamp: req.body.timestamp});

    note.save(function(err, data) {
        if(err) {
            console.log(err);
            res.status(500).send({message: "Some error occurred while creating the Note."});
        } else {
            res.send(data);
        }
    });
};

//create a collection of records
exports.createList = function(req, res) {
  if(!req.body.title) {
      return res.status(400).send({message: "Title cannot be empty"});
  }
  let array = req.body.meranie;
  if(array.length == 0) {
    return res.status(400).send({message: "No measured data"});
  }
  let notelist = new NoteList({title: req.body.title || "Untitled Note",
                               timestamp: req.body.timestamp,
                               children: [] })
  // console.log(notelist);
  for (let i = 0; i < array.length; i++) {
    notelist.children.push({acc: array[i]["acc"], mag: array[i]["mag"], timestamp: array[i]["timestamp"]});
  }
   let lastItem = notelist.children[array.length - 1];
   // console.log("lastItem: ",lastItem);
  compassData.x = lastItem.mag.mX;
  compassData.y = lastItem.mag.mY;
  compassData.z = lastItem.mag.mZ;

  // save received data collection to DB
  notelist.save(function(err, data) {
      if(err) {
          console.log(err);
          res.status(500).send({message: "Some error occurred while creating the Note."});
      } else {
          res.send(data);
      }
  });
}

//return last received data from amagentometer
//called from compass.js
exports.getCompassData = function() {
  return compassData;
}

exports.findAll = function(req, res) {
    // Retrieve and return all notes from the database.
    NoteList.find(function(err, notes){
        if(err) {
            console.log(err);
            res.status(500).send({message: "Some error occurred while retrieving notes."});
        } else {
            res.send(notes);
        }
    });
};

exports.findOne = function(req, res) {
  console.log("findOne", req.params.time);
  //find a collection from measurement with specific timestamp
  let collection = {};
  NoteList.
    find({'title':'meranickoRanicko', 'timestamp': req.params.time},
      function(err, note) {
        if(err) {
          console.log(err);
          // if(err.kind === 'ObjectId') {
          //     return res.status(404).send({message: "Note not found with timestamp: "+ req.params.time});
          // }
          return res.status(500).send({message: "Error retrieving note with timestamp: "+ req.params.time});
        }
        if(!note) {
          return res.status(404).send({message: "Note not found with timestamp: " + req.params.time});
        }
        res.send(note);
    });
};

// TODO: nefunguje pre moju databazu
exports.update = function(req, res) {
    // Update a note identified by the noteId in the request
    Note.findById(req.params.noteId, function(err, note) {
        if(err) {
            console.log(err);
            if(err.kind === 'ObjectId') {
                return res.status(404).send({message: "Note not found with id " + req.params.noteId});
            }
            return res.status(500).send({message: "Error finding note with id " + req.params.noteId});
        }

        if(!note) {
            return res.status(404).send({message: "Note not found with id " + req.params.noteId});
        }

        note.title = req.body.title;
        note.content = req.body.content;

        note.save(function(err, data){
            if(err) {
                res.status(500).send({message: "Could not update note with id " + req.params.noteId});
            } else {
                res.send(data);
            }
        });
    });
};

// TODO: nefunguje pre moju databazu
exports.delete = function(req, res) {
    // Delete a note with the specified noteId in the request
    Note.findByIdAndRemove(req.params.noteId, function(err, note) {
        if(err) {
            console.log(err);
            if(err.kind === 'ObjectId') {
                return res.status(404).send({message: "Note not found with id " + req.params.noteId});
            }
            return res.status(500).send({message: "Could not delete note with id " + req.params.noteId});
        }

        if(!note) {
            return res.status(404).send({message: "Note not found with id " + req.params.noteId});
        }

        res.send({message: "Note deleted successfully!"})
    });
};
