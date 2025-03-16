var Clay;
module.exports = function(minified) {
  Clay = this;
  var _ = minified._;
  var $ = minified.$;
  var HTML = minified.HTML;
  
  // array with timezones information
  var timezoneInfos = [
    {
      timezoneName: "BJS",
      timezoneLat: "39.90615",
      timezoneLng: "116.39125",
      timezoneDefaultOffset: "480"
    },
    {
      timezoneName: "BRU",
      timezoneLat: "50.84683",
      timezoneLng: "4.35170",
      timezoneDefaultOffset: "60"
    },
    {
      timezoneName: "BUE",
      timezoneLat: "-34.61607",
      timezoneLng: "-58.43329",
      timezoneDefaultOffset: "-180"
    },
    {
      timezoneName: "CAI",
      timezoneLat: "30.03503",
      timezoneLng: "31.56475",
      timezoneDefaultOffset: "130"
    },
    {
      timezoneName: "GMT",
      timezoneLat: "0",
      timezoneLng: "0",
      timezoneDefaultOffset: "0"
    },
    {
      timezoneName: "JKT",
      timezoneLat: "-6.17542",
      timezoneLng: "106.82718",
      timezoneDefaultOffset: "420"
    },
    {
      timezoneName: "HKG",
      timezoneLat: "22.27942",
      timezoneLng: "114.16281",
      timezoneDefaultOffset: "480"
    },
    {
      timezoneName: "KIE",
      timezoneLat: "50.4501",
      timezoneLng: "30.5234",
      timezoneDefaultOffset: "180"
    },    
    {
      timezoneName: "LON",
      timezoneLat: "51.5074",
      timezoneLng: "-0.1278",
      timezoneDefaultOffset: "60"
    },
    {
      timezoneName: "LAX",
      timezoneLat: "34.02106",
      timezoneLng: "-118.41174",
      timezoneDefaultOffset: "-480"
    },
    {
      timezoneName: "MAD",
      timezoneLat: "40.47806",
      timezoneLng: "-3.70343",
      timezoneDefaultOffset: "60"
    },
    {
      timezoneName: "MEL",
      timezoneLat: "-37.81439",
      timezoneLng: "144.96316",
      timezoneDefaultOffset: "-600"
    },
    {
      timezoneName: "MEX",
      timezoneLat: "19.43261",
      timezoneLng: "-99.13321",
      timezoneDefaultOffset: "-360"
    },
    {
      timezoneName: "MOW",
      timezoneLat: "55.72520",
      timezoneLng: "37.62896",
      timezoneDefaultOffset: "180"
    },
    {
      timezoneName: "DEL",
      timezoneLat: "28.64431",
      timezoneLng: "77.09239",
      timezoneDefaultOffset: "330"
    },
    {
      timezoneName: "NYC",
      timezoneLat: "40.69785",
      timezoneLng: "-73.97963",
      timezoneDefaultOffset: "-300"
    },
    {
      timezoneName: "NZD",
      timezoneLat: "-36.8485",
      timezoneLng: "174.7633",
      timezoneDefaultOffset: "720"
    },
    {
      timezoneName: "PAR",
      timezoneLat: "48.85888",
      timezoneLng: "2.34694",
      timezoneDefaultOffset: "60"
    },
    {
      timezoneName: "RIO",
      timezoneLat: "-22.9068",
      timezoneLng: "-43.1729",
      timezoneDefaultOffset: "-180"
    },    
    {
      timezoneName: "ROM",
      timezoneLat: "41.89880",
      timezoneLng: "12.54513",
      timezoneDefaultOffset: "60"
    },
    {
      timezoneName: "SLC",
      timezoneLat: "40.7608",
      timezoneLng: "-111.8910",
      timezoneDefaultOffset: "-360"
    },    
    {
      timezoneName: "SYD",
      timezoneLat: "-33.76965",
      timezoneLng: "150.80178",
      timezoneDefaultOffset: "600"
    },
    {
      timezoneName: "TYO",
      timezoneLat: "35.6895",
      timezoneLng: "139.6917",
      timezoneDefaultOffset: "540"
    },
    {
      timezoneName: "DCA",
      timezoneLat: "38.89381",
      timezoneLng: "-77.01457",
      timezoneDefaultOffset: "-300"
    }   
    
  ];
  
  
  
   //masterkey lookup
  function masterkeyLookup() {
    
    $.request('get', 'https://pmkey.xyz/search/?email=' + Clay.getItemById('MASTER_KEY_EMAIL').get() + '&pin=' + Clay.getItemById('MASTER_KEY_PIN').get()).then(function (str_data) {
       var data = JSON.parse(str_data);
      
      if (data.success) {
           Clay.getItemById('FORECAST_API_KEY').set(data.keys.weather.forecast);
           Clay.getItemById('OWM_API_KEY').set(data.keys.weather.owm);
                      
           Clay.getItemById('WU_API_KEY').set(data.keys.weather.wu);
      } else {
          alert(data.error);
      }

     
      
       });
  
  }
  
  
  
  
  //location coords lookup
  function coordinatesLookup() {
    
    $.request('get', 'http://nominatim.openstreetmap.org/search?q=' + Clay.getItemById('LOCATION_NAME').get() + '&format=json').then(function (str_data) {
       var result;
       var resultText='';
       var data = JSON.parse(str_data);
      
       //console.log(data);

       if (data.length === 0) { // no results
          resultText = '<div>No Results</div>';
       } else { //looping thru results
          resultText = '<div><i>Tap location below to get coordinates</i></div><hr/>';

         for (var i = 0; i < data.length; i++) {
            result = data[i];
            resultText += '<div onclick = "Clay.getItemById(\'LOCATION_LATITUDE\').set(\'' + parseInt(result.lat * 100000) + '\'); + Clay.getItemById(\'LOCATION_LONGITUDE\').set(\'' + parseInt(result.lon * 100000) + '\');">' +
                 (result.display_name.length > 37 ? result.display_name.substring(0, 34) + '...' : result.display_name) +
            '</div>';
         }
       }
      
       Clay.getItemById('LOCATION_RESULT').set(resultText);
       Clay.getItemById('LOCATION_NAME').set('');
      
       });
  
  }
  
  //weather location
  function changeWeatherLocation() {
    if (this.get() == '0') { // Automatic
      Clay.getItemById('LOCATION_LATITUDE').hide();
      Clay.getItemById('LOCATION_LONGITUDE').hide();
      Clay.getItemById('LOCATION_RESULT').hide();
      Clay.getItemById('LOCATION_NAME').hide();
      Clay.getItemById('LOCATION_BUTTON').hide();
      
      Clay.getItemById('LOCATION_LATITUDE').$manipulatorTarget.set('required', '');
      Clay.getItemById('LOCATION_LONGITUDE').$manipulatorTarget.set('required', '');
    } else { // Manual
      Clay.getItemById('LOCATION_LATITUDE').show();
      Clay.getItemById('LOCATION_LONGITUDE').show();
      Clay.getItemById('LOCATION_RESULT').show();
      Clay.getItemById('LOCATION_NAME').show();
      Clay.getItemById('LOCATION_BUTTON').show();
      
      Clay.getItemById('LOCATION_LATITUDE').$manipulatorTarget.set('required', 'required');
      Clay.getItemById('LOCATION_LONGITUDE').$manipulatorTarget.set('required', 'required');
     }
  }
  
  
  function showHideHoursMinutesSeparator() {
      if (((Clay.meta.activeWatchInfo && Clay.meta.activeWatchInfo.platform !== 'aplite') && (Clay.getItemById('SECONDARY_INFO_1').get() == '8' || Clay.getItemById('SECONDARY_INFO_2').get() == '8' || Clay.getItemById('SECONDARY_INFO_3').get() == '8' || 
         Clay.getItemById('SECONDARY_INFO_4').get() == '8' || Clay.getItemById('SECONDARY_INFO_5').get() == '8' || Clay.getItemById('SECONDARY_INFO_6').get() == '8' ||
          Clay.getItemById('SECONDARY_INFO_1').get() == '10' || Clay.getItemById('SECONDARY_INFO_2').get() == '10' || Clay.getItemById('SECONDARY_INFO_3').get() == '10' || 
         Clay.getItemById('SECONDARY_INFO_4').get() == '10' || Clay.getItemById('SECONDARY_INFO_5').get() == '10' || Clay.getItemById('SECONDARY_INFO_6').get() == '10') &&  (Clay.getItemByMessageKey('TIME_DISPLAY').get() != '2')) ||
         (Clay.getItemByMessageKey('TIME_DISPLAY').get() == '1')) {
             Clay.getItemById('HOURS_MINUTES_SEPARATOR').show();
     } else {
             Clay.getItemById('HOURS_MINUTES_SEPARATOR').hide();
     }
  }
  
  //secondary info
  function showHideSecondaryInfoOptions (){
     if (Clay.getItemById('SECONDARY_INFO_1').get() == '3' || Clay.getItemById('SECONDARY_INFO_2').get() == '3' || Clay.getItemById('SECONDARY_INFO_3').get() == '3' || 
         Clay.getItemById('SECONDARY_INFO_4').get() == '3' || Clay.getItemById('SECONDARY_INFO_5').get() == '3' || Clay.getItemById('SECONDARY_INFO_6').get() == '3' || 
         (Clay.getItemById('SECONDARY_INFO_7') &&  Clay.getItemById('SECONDARY_INFO_7').get() == '3') || (Clay.getItemById('SECONDARY_INFO_8') &&  Clay.getItemById('SECONDARY_INFO_8').get() == '3')){
             Clay.getItemById('RANDOM_TEXT').show();
     } else {
             Clay.getItemById('RANDOM_TEXT').hide();
     }
    
    if (Clay.getItemById('SECONDARY_INFO_1').get() == '8' || Clay.getItemById('SECONDARY_INFO_2').get() == '8' || Clay.getItemById('SECONDARY_INFO_3').get() == '8' || 
         Clay.getItemById('SECONDARY_INFO_4').get() == '8' || Clay.getItemById('SECONDARY_INFO_5').get() == '8' || Clay.getItemById('SECONDARY_INFO_6').get() == '8' || 
         (Clay.getItemById('SECONDARY_INFO_7') &&  Clay.getItemById('SECONDARY_INFO_7').get() == '8') || (Clay.getItemById('SECONDARY_INFO_8') &&  Clay.getItemById('SECONDARY_INFO_8').get() == '8')){
      
             Clay.getItemById('ALT_TIMEZONE_NAME').show();
     } else {
             Clay.getItemById('ALT_TIMEZONE_NAME').hide();
     }
  }
  
  
  function changeSecondaryInfo() {
    showHideSecondaryInfoOptions ();
    showHideHoursMinutesSeparator();
  }
  
  //alt timezone info
  function changeTimezone() {
    var currentTimezoneName = this.get(); // getting timezone name from SELECT
    var currentTimezoneInfo = timezoneInfos.filter(function(timezoneInfo) { // getting timezone info from array
        return timezoneInfo.timezoneName == currentTimezoneName;
    });
    
    var currentTimezoneOffset = currentTimezoneInfo[0].timezoneDefaultOffset; // assigning default timezone offset
    
    // trying to get realtime offset
    if (!(currentTimezoneInfo[0].timezoneLat == "0" && currentTimezoneInfo[0].timezoneLng == "0")) {
      var url = 'https://api.timezonedb.com/v2/get-time-zone?key=2F2TELTVDRIB&fields=gmtOffset&format=json&by=position&lat=' + currentTimezoneInfo[0].timezoneLat + '&lng=' + currentTimezoneInfo[0].timezoneLng;
      console.log(url);
      $.request('get', url).then(function (str_data) {
         var data = JSON.parse(str_data);
         if (data.status == 'OK') {
            currentTimezoneOffset = data.gmtOffset / 60; // got realtime API!
        }
      }).always(function() {
          // contining here
          Clay.getItemById('ALT_TIMEZONE_OFFSET').set(currentTimezoneOffset);
      });
    } else {
      Clay.getItemById('ALT_TIMEZONE_OFFSET').set(currentTimezoneOffset);
    }
    
  }
  
  
   //time display
  function changeTimeDisplay() {
    if (this.get() == '2') { // Digital full time - hide options
      
      if (Clay.meta.activeWatchInfo && Clay.meta.activeWatchInfo.platform !== 'aplite') {
        
        // applies to all platform
        Clay.getItemById('SECONDARY_INFO_2').hide(); 
        Clay.getItemById('SECONDARY_INFO_5').hide();
        
        // only not on chalk hide/show the rest
        if (Clay.meta.activeWatchInfo.platform !== 'chalk') {
           Clay.getItemById('ADDITIONAL_INFO_HEADER').hide();
           Clay.getItemById('SECONDARY_INFO_1').hide();
           Clay.getItemById('SECONDARY_INFO_3').hide();
           Clay.getItemById('SECONDARY_INFO_4').hide();
           Clay.getItemById('SECONDARY_INFO_6').hide();
           if (Clay.getItemById('SECONDARY_INFO_7')) Clay.getItemById('SECONDARY_INFO_7').hide();
           if (Clay.getItemById('SECONDARY_INFO_8')) Clay.getItemById('SECONDARY_INFO_8').hide();
           Clay.getItemById('ALT_TIMEZONE_NAME').hide();
           Clay.getItemById('RANDOM_TEXT').hide();
        } else {
           // show side-widgets if available
          if (Clay.getItemById('SECONDARY_INFO_9')) Clay.getItemById('SECONDARY_INFO_9').show();
          if (Clay.getItemById('SECONDARY_INFO_10')) Clay.getItemById('SECONDARY_INFO_10').show();
        }
      }
      

        
       Clay.getItemById('SHOW_ANALOG_SECONDS').set(0);
       Clay.getItemById('SHOW_ANALOG_SECONDS').hide();
      
    } else { //otherwise
      
        if (Clay.meta.activeWatchInfo && Clay.meta.activeWatchInfo.platform !== 'aplite') {
          
           // applies to all platform
           Clay.getItemById('SECONDARY_INFO_2').show();  
           Clay.getItemById('SECONDARY_INFO_5').show();
          
          // only not on chalk hide the rest
           if (Clay.meta.activeWatchInfo.platform !== 'chalk') {
             Clay.getItemById('ADDITIONAL_INFO_HEADER').show();
             Clay.getItemById('SECONDARY_INFO_1').show();
             Clay.getItemById('SECONDARY_INFO_3').show();
             Clay.getItemById('SECONDARY_INFO_4').show();
             Clay.getItemById('SECONDARY_INFO_6').show();
             if (Clay.getItemById('SECONDARY_INFO_7')) Clay.getItemById('SECONDARY_INFO_7').show();
             if (Clay.getItemById('SECONDARY_INFO_8')) Clay.getItemById('SECONDARY_INFO_8').show();
             showHideSecondaryInfoOptions ();
           } else {
              // hide side-widgets if available
             if (Clay.getItemById('SECONDARY_INFO_9')) Clay.getItemById('SECONDARY_INFO_9').hide();
             if (Clay.getItemById('SECONDARY_INFO_10')) Clay.getItemById('SECONDARY_INFO_10').hide();
           }
        }
      
       if (this.get() == '0') {  //in analog mode show Second Hand option
         Clay.getItemById('SHOW_ANALOG_SECONDS').show();
       } else {
         Clay.getItemById('SHOW_ANALOG_SECONDS').set(0);
         Clay.getItemById('SHOW_ANALOG_SECONDS').hide();
       }
      
     }
    
      showHideHoursMinutesSeparator();
  }
  
  
  var need_to_reset_themes = true; // whether need to reset predefined theme selection
  function toggleColorTheme() {
    if (this.get() != ' ') { // theme is set
      
      var colors = this.get().split(","); // getting colors from SELECT value
      
      need_to_reset_themes = false; // on automatic color set - don't reset theme
      Clay.getItemByMessageKey('PRIMARY_COLOR').set(colors[0]);
      Clay.getItemByMessageKey('SECONDARY_COLOR').set(colors[1]);
      Clay.getItemByMessageKey('ICON_COLOR').set(colors[2]);
      Clay.getItemByMessageKey('BACK_COLOR').set(colors[3]);
        
      need_to_reset_themes = true;
      
    }
  }
  
  
  function resetColorTheme() {
    if (need_to_reset_themes) Clay.getItemByMessageKey('COLOR_THEME').set(' ');
  }
  
  
  function toggleGraphicalStepGoal(){
    if (!this.get()){
        Clay.getItemByMessageKey('CUSTOM_STEP_GOAL').set("");
        Clay.getItemByMessageKey('CUSTOM_STEP_GOAL').hide();
    } else {
        Clay.getItemByMessageKey('CUSTOM_STEP_GOAL').show();
    }
  }
  
   Clay.on(Clay.EVENTS.AFTER_BUILD, function() {
     
    // weather location service
    var weatherLocation = Clay.getItemByMessageKey('LOCATION_SERVICE');
    changeWeatherLocation.call(weatherLocation);
    weatherLocation.on('change', changeWeatherLocation);
     
     
    // on clicking Coordinates lookup button - perform coordinates lookup
    Clay.getItemById('LOCATION_BUTTON').on('click', coordinatesLookup);
     
     
    // on clicking MasterKey lookup button - perform masterkey lookup
    Clay.getItemById('MASTER_KEY_BUTTON').on('click', masterkeyLookup);  
     
      // time display
    var timeDisplay = Clay.getItemByMessageKey('TIME_DISPLAY');
    changeTimeDisplay.call(timeDisplay);
    timeDisplay.on('change', changeTimeDisplay);  
     
    if (Clay.meta.activeWatchInfo && Clay.meta.activeWatchInfo.platform !== 'aplite') { 
      
        // initially hiding BUY NOW 
           var buynow = Clay.getItemByMessageKey('BUY_NOW');
           buynow.set(0); 
           buynow.disable();
           buynow.hide(); 
    

      
      
      
        //hiding hidden fields
        Clay.getItemById('ALT_TIMEZONE_OFFSET').hide(); 
         
        //alt timezone info
        var timezoneService = Clay.getItemByMessageKey('ALT_TIMEZONE_NAME');
        changeTimezone.call(timezoneService);
        timezoneService.on('change', changeTimezone);  
         
        // secondary info
        Clay.getItemByMessageKey('SECONDARY_INFO_1').on('change',changeSecondaryInfo);
        Clay.getItemByMessageKey('SECONDARY_INFO_2').on('change',changeSecondaryInfo);
        Clay.getItemByMessageKey('SECONDARY_INFO_3').on('change',changeSecondaryInfo);
        Clay.getItemByMessageKey('SECONDARY_INFO_4').on('change',changeSecondaryInfo);
        Clay.getItemByMessageKey('SECONDARY_INFO_5').on('change',changeSecondaryInfo);
        Clay.getItemByMessageKey('SECONDARY_INFO_6').on('change',changeSecondaryInfo);
        if (Clay.getItemByMessageKey('SECONDARY_INFO_7')) Clay.getItemByMessageKey('SECONDARY_INFO_7').on('change',changeSecondaryInfo);
        if (Clay.getItemByMessageKey('SECONDARY_INFO_8')) Clay.getItemByMessageKey('SECONDARY_INFO_8').on('change',changeSecondaryInfo);
        changeSecondaryInfo.call(Clay.getItemByMessageKey('SECONDARY_INFO_1'));
      
      
        // resetting theme on custom color select
        Clay.getItemByMessageKey('PRIMARY_COLOR').on('change', resetColorTheme);
        Clay.getItemByMessageKey('SECONDARY_COLOR').on('change', resetColorTheme);
        Clay.getItemByMessageKey('ICON_COLOR').on('change', resetColorTheme);
        Clay.getItemByMessageKey('BACK_COLOR').on('change', resetColorTheme);       
      
        // colorThemes
        var colorThemeSelection = Clay.getItemByMessageKey('COLOR_THEME');
        toggleColorTheme.call(colorThemeSelection);
        colorThemeSelection.on('change', toggleColorTheme);
      
        // colorThemes
        var graphicalStepGoal = Clay.getItemByMessageKey('GRAPHICAL_STEP_GOAL');
        toggleGraphicalStepGoal.call(graphicalStepGoal);
        graphicalStepGoal.on('change', toggleGraphicalStepGoal);
      
    }
     
     
   });
  
 
};