// Configuring the database
var dbConfig = require('./config/database.config.js');
var mongoose = require('mongoose');
let express = require('express');
let expressPublicIp = require('express-public-ip');
let bodyParser = require('body-parser');



mongoose.Promise = global.Promise;

mongoose.connect(dbConfig.url);

mongoose.connection.on('error', function() {
    console.log('Could not connect to the database. Exiting now...');
    process.exit();
});

mongoose.connection.once('open', function() {
    console.log("Successfully connected to the database");
})


// create express app
let app = express();

app.enable('trust proxy');
app.use(expressPublicIp());

// parse requests of content-type - application/x-www-form-urlencoded
app.use(bodyParser.urlencoded({ extended: true }))

// parse requests of content-type - application/json
app.use(bodyParser.json())

// define a simple route
app.get('/', function(req, res){
    res.json({"message": "Welcome to application."});
});

// Require Notes routes
require('./app/routes/note.routes.js')(app);

// listen for requests
app.listen(3000, function(){
    console.log("Server is listening on port 3000");
});
