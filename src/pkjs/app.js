var Clay = require('pebble-clay');
var customClay = require('./custom-clay');
var clayConfig = require('./config');
var clay; //constructor moved to JS Ready


var GenericWeather = require('pebble-generic-weather');
var genericWeather = new GenericWeather();

require('message_keys');


/* Add your own js code below */




Pebble.addEventListener('ready', function(e) {
 
  // initiating Clay with custom functions & account info
  clay = new Clay(clayConfig, customClay, {userData: {appid: '724227591', accounttoken: Pebble.getAccountToken()}});
   
  console.log('PebbleKit JS ready!');
  
    var dictionary = {
      "JSREADY": 1
    };

    Pebble.sendAppMessage(dictionary,
            function(e) {
              console.log ("\n++++ I am inside of 'Pebble.sendAppMessage() callback: Ready notice sent to phone successfully!");
            },
            function(e) {
             console.log ("\n++++ I am inside of 'Pebble.sendAppMessage() callback: Error ready notice to Pebble! = " +  JSON.stringify(e));
            }
          ); 
      
    }
);

// Get AppMessage events
Pebble.addEventListener('appmessage', function(e) {
  console.log('appmessage: ' + JSON.stringify(e.payload));
  genericWeather.appMessageHandler(e);
});
