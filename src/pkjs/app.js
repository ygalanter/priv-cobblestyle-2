var Clay = require('@rebble/clay');
var customClay = require('./custom-clay');
var clayConfig = require('./config');
var clay; //constructor moved to JS Ready

require('message_keys');

// Weather status codes -- must match the WEATHER_STATUS_* defines in common.h
var WEATHER_STATUS_AVAILABLE = 0;
var WEATHER_STATUS_FAILED = 2;
var WEATHER_STATUS_LOCATION_UNAVAILABLE = 3;

// Location service modes -- must match LOCATION_SERVICE_* in common.h
var LOCATION_SERVICE_MANUAL = 1;


function sendWeatherError(status) {
  Pebble.sendAppMessage({ 'WEATHER_STATUS': status });
}

function sendWeather(temp, code, isDay, city) {
  Pebble.sendAppMessage({
    'WEATHER_STATUS': WEATHER_STATUS_AVAILABLE,
    'WEATHER_TEMP': temp,
    'WEATHER_CODE': code,
    'WEATHER_IS_DAY': isDay,
    'WEATHER_CITY': city
  });
}

// Reverse-geocodes lat/lon to a human town name via OpenStreetMap Nominatim,
// then sends the full weather packet to the watch (name is best-effort).
function fetchCityThenSend(latitude, longitude, temp, code, isDay) {
  var url = 'https://nominatim.openstreetmap.org/reverse?lat=' + latitude +
            '&lon=' + longitude + '&format=json&accept-language=en-US';
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    var city = 'N/A';
    try {
      var a = JSON.parse(this.responseText).address;
      city = a.city || a.town || a.village || a.hamlet ||
             a.county || a.state || a.country || 'N/A';
    } catch (e) {
      console.log('Nominatim parse error: ' + e);
    }
    sendWeather(temp, code, isDay, city);
  };
  xhr.onerror = function () { sendWeather(temp, code, isDay, 'N/A'); };
  xhr.ontimeout = function () { sendWeather(temp, code, isDay, 'N/A'); };
  xhr.open('GET', url);
  // Nominatim requires an identifying User-Agent (honored on Android; iOS blocks it).
  try { xhr.setRequestHeader('User-Agent', 'CobbleStyle2 Pebble Watchface'); } catch (e) {}
  xhr.send();
}

// Fetches current conditions from Open-Meteo (no API key). Temperature is always
// requested in Celsius; the watch derives Fahrenheit/Kelvin locally.
function fetchWeatherAt(latitude, longitude) {
  var url = 'https://api.open-meteo.com/v1/forecast?latitude=' + latitude +
            '&longitude=' + longitude +
            '&current=temperature_2m,weather_code,is_day';
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    try {
      var c = JSON.parse(this.responseText).current;
      fetchCityThenSend(latitude, longitude,
                        Math.round(c.temperature_2m), c.weather_code, c.is_day);
    } catch (e) {
      console.log('Open-Meteo parse error: ' + e);
      sendWeatherError(WEATHER_STATUS_FAILED);
    }
  };
  xhr.onerror = function () { sendWeatherError(WEATHER_STATUS_FAILED); };
  xhr.ontimeout = function () { sendWeatherError(WEATHER_STATUS_FAILED); };
  xhr.open('GET', url);
  xhr.send();
}

// Resolves the location (GPS or the manual coordinates the watch supplied,
// stored as degrees x100000) and kicks off a weather fetch.
function getWeather(payload) {
  if (payload && payload.LOCATION_SERVICE === LOCATION_SERVICE_MANUAL) {
    fetchWeatherAt(payload.LOCATION_LATITUDE / 100000, payload.LOCATION_LONGITUDE / 100000);
  } else {
    navigator.geolocation.getCurrentPosition(
      function (pos) { fetchWeatherAt(pos.coords.latitude, pos.coords.longitude); },
      function (err) {
        console.log('Location error: ' + JSON.stringify(err));
        sendWeatherError(WEATHER_STATUS_LOCATION_UNAVAILABLE);
      },
      { timeout: 15000, maximumAge: 60000 }
    );
  }
}


Pebble.addEventListener('ready', function(e) {

  // initiating Clay with custom functions & account info
  clay = new Clay(clayConfig, customClay, {userData: {appid: '724227591', accounttoken: Pebble.getAccountToken()}});

  console.log('PebbleKit JS ready!');

  // tell the watch we're up; it replies with a WEATHER_REQUEST carrying the
  // current location settings, which drives the first fetch.
  Pebble.sendAppMessage({ 'JSREADY': 1 });
});

// Get AppMessage events
Pebble.addEventListener('appmessage', function(e) {
  if (e.payload && e.payload.WEATHER_REQUEST) {
    getWeather(e.payload);
  }
});
