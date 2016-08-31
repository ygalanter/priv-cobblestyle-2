module.exports = [
  
  
    //***************** END TRIAAL, BUY NOW *****************
    {
    "type": "section",
    "items": [
  
        {
            "type": "toggle",
            "id": "BUY_NOW",  // To end trial earlier and initiate purchase righ away
            "messageKey": "BUY_NOW",
            "defaultValue": 0,
            "label": "END TRIAL, BUY NOW",
            "description": "Toggle to end your trial initiate purchase. After saving config follow instructions on your watch",
            "capabilities": ["NOT_PLATFORM_APLITE"]
        }
      
     ]
    },
  
  
  
  
   //***************** APPEARANCE RECT *****************
    {
    "type": "section",
    "capabilities": ["RECT"],
    "items": [
        {
          "type": "heading",
          "defaultValue": "Appearance"
        },
       {
          "type": "select",
          "id": "TIME_DISPLAY",       
          "messageKey": "TIME_DISPLAY",
          "defaultValue": "1",
          "label": "Time Display",
          "options": [
            { 
              "label": "Analog Mode", 
              "value": "0"
            },
            { 
              "label": "Digital Mode", 
              "value": "1" 
            },
            { 
              "label": "Big Time Mode", 
              "value": "2" 
            }
          ]
        },
       {
          "type": "select",
          "id": "HOURS_MINUTES_SEPARATOR",       
          "messageKey": "HOURS_MINUTES_SEPARATOR",
          "defaultValue": ":",
          "label": "Hours-Minutes separator",
          "options": [
            { 
              "label": "Colon  : ", 
              "value": ":"
            },
            { 
              "label": "Dot  . ", 
              "value": "."
            },
            { 
              "label": "Dash  - ", 
              "value": "-"
            },
            { 
              "label": "Space    ", 
              "value": " "
            }
            
          ]
        },
        {
          "type": "toggle",
          "id": "SHOW_ANALOG_SECONDS", 
          "messageKey": "SHOW_ANALOG_SECONDS",
          "defaultValue": 0,
          "label": "Show second hand"
        },
      
        
      
        {
          "type": "toggle",
          "id": "GRAPHICAL_STEP_GOAL", 
          "messageKey": "GRAPHICAL_STEP_GOAL",
          "defaultValue": 0,
          "label": "Display step goal bar",
          "capabilities": ["HEALTH"] // rect watches
        },
        
     
        {
            "type": "input",                    
            "messageKey": "CUSTOM_STEP_GOAL",
            "id":  "CUSTOM_STEP_GOAL",
            "defaultValue": "",
            "label": "Custom Step Goal",
            "capabilities": ["HEALTH"],
            "attributes": {"type": "number"}
         },
      
        {
          "type": "select",
          "id": "SIDEBAR_LOCATION",       
          "messageKey": "SIDEBAR_LOCATION",
          "defaultValue": "0",
          "label": "Sidebar Location",
          "options": [
            { 
              "label": "Right", 
              "value": "0" // #48 (- 48) = 0 
            },
            { 
              "label": "Left", 
              "value": "1" // #49 (- 48) = 1
            }
          ]
        }
      
      ]
    },
  
  //******************************* Apperance ROUND ***********************
  
   {
    "type": "section",
    "capabilities": ["ROUND"],
    "items": [
        {
          "type": "heading",
          "defaultValue": "Circle Information"
        },
       {
          "type": "select",
          "id": "TIME_DISPLAY",       
          "messageKey": "TIME_DISPLAY",
          "defaultValue": "1",
          "label": "Time Display",
          "options": [
            { 
              "label": "Analog Mode", 
              "value": "0"
            },
            { 
              "label": "Digital Mode", 
              "value": "1" 
            },
            { 
              "label": "Big Time Mode", 
              "value": "2" 
            }
          ]
        },
       {
          "type": "select",
          "id": "HOURS_MINUTES_SEPARATOR",       
          "messageKey": "HOURS_MINUTES_SEPARATOR",
          "defaultValue": ":",
          "label": "Hours-Minutes separator",
          "options": [
            { 
              "label": "Colon  : ", 
              "value": ":"
            },
            { 
              "label": "Dot  . ", 
              "value": "."
            },
            { 
              "label": "Dash  - ", 
              "value": "-"
            },
            { 
              "label": "Space    ", 
              "value": " "
            }
            
          ]
        },
        {
          "type": "toggle",
          "id": "SHOW_ANALOG_SECONDS", 
          "messageKey": "SHOW_ANALOG_SECONDS",
          "defaultValue": 0,
          "label": "Show second hand"
        },
      
        
      {
          "type": "toggle",
          "id": "GRAPHICAL_STEP_GOAL", 
          "messageKey": "GRAPHICAL_STEP_GOAL",
          "defaultValue": 0,
          "label": "Display step goal circle",
          "capabilities": ["HEALTH"] // round watches
        },
      
      
      
        {
            "type": "input",                    
            "messageKey": "CUSTOM_STEP_GOAL",
            "id":  "CUSTOM_STEP_GOAL",
            "defaultValue": "",
            "label": "Custom Step Goal",
            "capabilities": ["HEALTH"],
            "attributes": {"type": "number"}
         },
      
       {
          "type": "select",
          "id": "SECONDARY_INFO_2",   
          "messageKey": "SECONDARY_INFO_2",
          "defaultValue": "9",
          "label": "Widget 1",
          "options": [
            { 
              "label": "Nothing", 
              "value": "9" // #55 (- 48) = 7
            },
            { 
              "label": "Local Time", 
              "value": "10" // #55 (- 48) = 7
            },
           
            { 
              "label": "Location", 
              "value": "0" // #48 (- 48) = 0 
            },
            { 
              "label": "AM/PM", 
              "value": "1" // #49 (- 48) = 1
            },
            { 
              "label": "Seconds", 
              "value": "2" // #50 (- 48) = 2
            },
            { 
              "label": "Alternative Timezone", 
              "value": "8" // #55 (- 48) = 7
            },
            { 
              "label": "Custom Text", 
              "value": "3" // #51 (- 48) = 3
            },
            { 
              "label": "Weather", 
              "value": "13" // #55 (- 48) = 7
            },
            { 
              "label": "Time Active", 
              "value": "16" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned at rest", 
              "value": "17" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned active", 
              "value": "18" // #52 (- 48) = 4
            },
            { 
              "label": "Step Count", 
              "value": "4" // #52 (- 48) = 4
            },
            { 
              "label": "Distance meters", 
              "value": "5" // #53 (- 48) = 5
            },
            { 
              "label": "Distance km", 
              "value": "6" // #54 (- 48) = 6
            },
            { 
              "label": "Distance miles", 
              "value": "7" // #55 (- 48) = 7
            }
          ]
        },
      
      {
          "type": "select",
          "id": "SECONDARY_INFO_5",   
          "messageKey": "SECONDARY_INFO_5",
          "defaultValue": "9",
          "label": "Widget 2",
          "options":  [
            { 
              "label": "Nothing", 
              "value": "9" // #55 (- 48) = 7
            },
            { 
              "label": "Local Time", 
              "value": "10" // #55 (- 48) = 7
            },
           
            { 
              "label": "Location", 
              "value": "0" // #48 (- 48) = 0 
            },
            { 
              "label": "AM/PM", 
              "value": "1" // #49 (- 48) = 1
            },
            { 
              "label": "Seconds", 
              "value": "2" // #50 (- 48) = 2
            },
            { 
              "label": "Alternative Timezone", 
              "value": "8" // #55 (- 48) = 7
            },
            { 
              "label": "Custom Text", 
              "value": "3" // #51 (- 48) = 3
            },
            { 
              "label": "Weather", 
              "value": "13" // #55 (- 48) = 7
            },
            { 
              "label": "Time Active", 
              "value": "16" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned at rest", 
              "value": "17" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned active", 
              "value": "18" // #52 (- 48) = 4
            },
            { 
              "label": "Step Count", 
              "value": "4" // #52 (- 48) = 4
            },
            { 
              "label": "Distance meters", 
              "value": "5" // #53 (- 48) = 5
            },
            { 
              "label": "Distance km", 
              "value": "6" // #54 (- 48) = 6
            },
            { 
              "label": "Distance miles", 
              "value": "7" // #55 (- 48) = 7
            }
            
          ]
        },
      
      {
          "type": "select",
          "id": "SECONDARY_INFO_9",   
          "messageKey": "SECONDARY_INFO_9",
          "defaultValue": "9",
          "label": "Widget 1",
          "options":  [
            { 
              "label": "Nothing", 
              "value": "9" // #55 (- 48) = 7
            },
            { 
              "label": "AM/PM", 
              "value": "1" // #55 (- 48) = 7
            },
           
            { 
              "label": "Seconds", 
              "value": "2" // #48 (- 48) = 0 
            },
           
            { 
              "label": "12H / 24H", 
              "value": "15" // #48 (- 48) = 0 
            }
            
          ]
        },
      
      {
          "type": "select",
          "id": "SECONDARY_INFO_10",   
          "messageKey": "SECONDARY_INFO_10",
          "defaultValue": "9",
          "label": "Widget 2",
          "options":  [
            { 
              "label": "Nothing", 
              "value": "9" // #55 (- 48) = 7
            },
            { 
              "label": "AM/PM", 
              "value": "1" // #55 (- 48) = 7
            },
           
            { 
              "label": "Seconds", 
              "value": "2" // #48 (- 48) = 0 
            },
           
            { 
              "label": "12H / 24H", 
              "value": "15" // #48 (- 48) = 0 
            }
            
          ]
        },
      
    
      
      {
          "type": "select",
          "id": "ALT_TIMEZONE_NAME",   
          "messageKey": "ALT_TIMEZONE_NAME",
          "defaultValue": "NYC",
          "label": "Alternative Timezone",
          "options": [
              { 
                "label": "Beijing Time", 
                "value": "BJS"  
              },
              { 
                "label": "Brussels Time", 
                "value": "BRU"  
              },
              { 
                "label": "Buenos Aires Time", 
                "value": "BUE"  
              },
              { 
                "label": "Cairo Time", 
                "value": "CAI"  
              },
              { 
                "label": "GMT Time", 
                "value": "GMT"  
              },
              { 
                "label": "Jakarta Time", 
                "value": "JKT"  
              },
              { 
                "label": "Hong Kong Time", 
                "value": "HKG"  
              },
              { 
                "label": "Kiev Tinme", 
                "value": "KIE"  
              },
              { 
                "label": "London Time", 
                "value": "LON"  
              },
              { 
                "label": "Los Angeles Time", 
                "value": "LAX"  
              },
              { 
                "label": "Madrid Time", 
                "value": "MAD"  
              },
              { 
                "label": "Melbourne Time", 
                "value": "MEL"  
              },
              { 
                "label": "Mexico City Time", 
                "value": "MEX"  
              },
              { 
                "label": "Moscow Time", 
                "value": "MOW"  
              },
              { 
                "label": "New Delhi Time", 
                "value": "DEL"  
              },
              { 
                "label": "New York Time", 
                "value": "NYC"  
              },
              { 
                "label": "New Zealand Time", 
                "value": "NZD"  
              },
              { 
                "label": "Paris Time", 
                "value": "PAR"  
              },
              { 
                "label": "Rio de Janeiro Time", 
                "value": "RIO"  
              },            
              { 
                "label": "Rome Time", 
                "value": "ROM"  
              },
              { 
                "label": "Salt Lake City Time", 
                "value": "SLC"  
              },
              { 
                "label": "Sydney Time", 
                "value": "SYD"  
              },
              { 
                "label": "Tokyo Time", 
                "value": "TYO"  
              },
              { 
                "label": "Washington Time", 
                "value": "DCA"  
              }
          ]
      },
      
      {
            "type": "input",                    
            "messageKey": "ALT_TIMEZONE_OFFSET",
            "id":  "ALT_TIMEZONE_OFFSET",
            "defaultValue": "0",
            "label": "Alternative Timezone offset (hidden)",
         }
      
      
      ]
    },
  
  // ********************************* Additional info ROUND ******************************
    {"type": "section",
    "capabilities": ["NOT_PLATFORM_APLITE", "ROUND"],
    "items": [
        {
          "id":"ADDITIONAL_INFO_HEADER",
          "type": "heading",
          "defaultValue": "Ring Information"
        },
      
         
      
       {
          "type": "select",
          "id": "SECONDARY_INFO_1",   
          "messageKey": "SECONDARY_INFO_1",
          "defaultValue": "11",
          "label": "Option 1",
          "options":  [
            { 
              "label": "Nothing", 
              "value": "9" // #55 (- 48) = 7
            },
             { 
              "label": "Day and Month", 
              "value": "12" // #55 (- 48) = 7
            },
            { 
              "label": "Week Number", 
              "value": "11" // #49 (- 48) = 1
            },
            { 
              "label": "Weather", 
              "value": "13" // #55 (- 48) = 7
            },
            { 
              "label": "Time Active", 
              "value": "16" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned at rest", 
              "value": "17" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned active", 
              "value": "18" // #52 (- 48) = 4
            },
            { 
              "label": "Step Count", 
              "value": "4" // #52 (- 48) = 4
            },
            { 
              "label": "Distance meters", 
              "value": "5" // #53 (- 48) = 5
            },
            { 
              "label": "Distance km", 
              "value": "6" // #54 (- 48) = 6
            },
            { 
              "label": "Distance miles", 
              "value": "7" // #55 (- 48) = 7
            }
            
          ]
        },
      
        {
              "type": "select",
              "id": "SECONDARY_INFO_8",   
              "messageKey": "SECONDARY_INFO_8",
              "defaultValue": "14",
              "label": "Option 2",
              "options": [
            { 
              "label": "Nothing", 
              "value": "9" // #55 (- 48) = 7
            },
            { 
              "label": "Location", 
              "value": "0" // #48 (- 48) = 0 
            },
            { 
              "label": "Local Time", 
              "value": "10" // #55 (- 48) = 7
            },
            { 
              "label": "Day of the Week", 
              "value": "14" // #55 (- 48) = 7
            },
            { 
              "label": "Alternative Timezone", 
              "value": "8" // #55 (- 48) = 7
            },
            { 
              "label": "Custom Text", 
              "value": "3" // #51 (- 48) = 3
            }
            
          ]
        },
      
      {
          "type": "select",
          "id": "SECONDARY_INFO_3",   
          "messageKey": "SECONDARY_INFO_3",
          "defaultValue": "12",
          "label": "Option 3",
          "options": [
            { 
              "label": "Nothing", 
              "value": "9" // #55 (- 48) = 7
            },
             { 
              "label": "Day and Month", 
              "value": "12" // #55 (- 48) = 7
            },
            { 
              "label": "Week Number", 
              "value": "11" // #49 (- 48) = 1
            },
            { 
              "label": "Weather", 
              "value": "13" // #55 (- 48) = 7
            },
            { 
              "label": "Time Active", 
              "value": "16" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned at rest", 
              "value": "17" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned active", 
              "value": "18" // #52 (- 48) = 4
            },
            { 
              "label": "Step Count", 
              "value": "4" // #52 (- 48) = 4
            },
            { 
              "label": "Distance meters", 
              "value": "5" // #53 (- 48) = 5
            },
            { 
              "label": "Distance km", 
              "value": "6" // #54 (- 48) = 6
            },
            { 
              "label": "Distance miles", 
              "value": "7" // #55 (- 48) = 7
            }
            
          ]
        },
      
       {
          "type": "select",
          "id": "SECONDARY_INFO_6",   
          "messageKey": "SECONDARY_INFO_6",
          "defaultValue": "13",
          "label": "Option 4",
          "options": [
            { 
              "label": "Nothing", 
              "value": "9" // #55 (- 48) = 7
            },
             { 
              "label": "Day and Month", 
              "value": "12" // #55 (- 48) = 7
            },
            { 
              "label": "Week Number", 
              "value": "11" // #49 (- 48) = 1
            },
            { 
              "label": "Weather", 
              "value": "13" // #55 (- 48) = 7
            },
            { 
              "label": "Time Active", 
              "value": "16" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned at rest", 
              "value": "17" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned active", 
              "value": "18" // #52 (- 48) = 4
            },
            { 
              "label": "Step Count", 
              "value": "4" // #52 (- 48) = 4
            },
            { 
              "label": "Distance meters", 
              "value": "5" // #53 (- 48) = 5
            },
            { 
              "label": "Distance km", 
              "value": "6" // #54 (- 48) = 6
            },
            { 
              "label": "Distance miles", 
              "value": "7" // #55 (- 48) = 7
            }
            
          ]
        },
      
       {
              "type": "select",
              "id": "SECONDARY_INFO_7",   
              "messageKey": "SECONDARY_INFO_7",
              "defaultValue": "0",
              "label": "Option 5",
              "options": [
            { 
              "label": "Nothing", 
              "value": "9" // #55 (- 48) = 7
            },
            { 
              "label": "Location", 
              "value": "0" // #48 (- 48) = 0 
            },
            { 
              "label": "Local Time", 
              "value": "10" // #55 (- 48) = 7
            },
            { 
              "label": "Day of the Week", 
              "value": "14" // #55 (- 48) = 7
            },
            { 
              "label": "Alternative Timezone", 
              "value": "8" // #55 (- 48) = 7
            },
            { 
              "label": "Custom Text", 
              "value": "3" // #51 (- 48) = 3
            }
            
          ]
        },
      
      
       {
          "type": "select",
          "id": "SECONDARY_INFO_4",   
          "messageKey": "SECONDARY_INFO_4",
          "defaultValue": "4",
          "label": "Option 6",
          "options": [
            { 
              "label": "Nothing", 
              "value": "9" // #55 (- 48) = 7
            },
             { 
              "label": "Day and Month", 
              "value": "12" // #55 (- 48) = 7
            },
            { 
              "label": "Week Number", 
              "value": "11" // #49 (- 48) = 1
            },
            { 
              "label": "Weather", 
              "value": "13" // #55 (- 48) = 7
            },
            { 
              "label": "Time Active", 
              "value": "16" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned at rest", 
              "value": "17" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned active", 
              "value": "18" // #52 (- 48) = 4
            },
            { 
              "label": "Step Count", 
              "value": "4" // #52 (- 48) = 4
            },
            { 
              "label": "Distance meters", 
              "value": "5" // #53 (- 48) = 5
            },
            { 
              "label": "Distance km", 
              "value": "6" // #54 (- 48) = 6
            },
            { 
              "label": "Distance miles", 
              "value": "7" // #55 (- 48) = 7
            }
            
          ]
        },
           
          
        {
            "type": "input",                    
            "messageKey": "RANDOM_TEXT",
            "id":  "RANDOM_TEXT",
            "defaultValue": "",
            "label": "Custom Text",
         }
  
    ]},
  
  
  // ****************************** Colors *******************************
  {"type": "section",
    "capabilities": ["NOT_PLATFORM_APLITE"],
    "items": [
        {
          "type": "heading",
          "defaultValue": "Colors",
         "capabilities": ["NOT_PLATFORM_APLITE"],
        },
       {
        "type": "select",
        "id": "COLOR_THEME",        // Predefined color theme
        "messageKey": "COLOR_THEME",
        "defaultValue": "555500,FFFF55,ffffff,000000,z" ,
        "label": "Color Theme",
        "capabilities": ["COLOR"],
        "options": [
          { 
            "label": "-select theme-", 
            "value": " " 
          },
          { 
            "label": "GREEN",
            "value": "005500,00FF55,ffffff,000000,z"  // z = not a gradient theme
          },
          { 
            "label": "RED",
            "value": "550000,FF55AA,ffffff,000000,z" 
          },
          { 
            "label": "BLUE",
            "value": "000055,55FFFF,ffffff,000000,z" 
          },
          { 
            "label": "YELLOW",
            "value": "555500,FFFF55,ffffff,000000,z" 
          },     
          { 
            "label": "PURPLE",
            "value": "550055,AA55FF,ffffff,000000,z" 
          },
          { 
            "label": "ORANGE",
            "value": "AA5500,FFAA55,ffffff,000000,z" 
          }
        ]
      },
      {
          "type": "color",                    // custom colors
          "messageKey": "PRIMARY_COLOR",
          "id": "PRIMARY_COLOR",   
          "defaultValue": "005500",
          "label": "Primary color",
          "sunlight": true,
          "allowGray": true
        }, 
        {
          "type": "color",                    
          "messageKey": "SECONDARY_COLOR",
          "id": "SECONDARY_COLOR",   
          "defaultValue": "00aa55",
          "label": "Secondary color",
          "sunlight": true,
          "allowGray": true
        }, 
        {
          "type": "color",                    
          "messageKey": "BACK_COLOR",
          "id": "BACK_COLOR",   
          "defaultValue": "000000",
          "label": "Background color",
          "sunlight": true,
          "allowGray": true
        }, 
        {
          "type": "color",                    
          "messageKey": "ICON_COLOR",
          "id": "ICON_COLOR",   
          "defaultValue": "ffffff",
          "label": "Icon color",
          "sunlight": true,
          "allowGray": true        }
     
       
      ]
  },
      
      
  // ********************************* Additional info RECT ******************************
    {"type": "section",
    "capabilities": ["NOT_PLATFORM_APLITE", "RECT"],
    "items": [
        {
          "id":"ADDITIONAL_INFO_HEADER",
          "type": "heading",
          "defaultValue": "Additional Information"
        },
       {
          "type": "select",
          "id": "SECONDARY_INFO_1",   
          "messageKey": "SECONDARY_INFO_1",
          "defaultValue": "9",
          "label": "Option 1",
          "options": [
            { 
              "label": "Nothing", 
              "value": "9" // #55 (- 48) = 7
            }, 
            { 
              "label": "Local Time", 
              "value": "10" // #55 (- 48) = 7
            },
           
            { 
              "label": "Location", 
              "value": "0" // #48 (- 48) = 0 
            },
            { 
              "label": "Week Number", 
              "value": "11" // #49 (- 48) = 1
            },
            { 
              "label": "Day of the Week", 
              "value": "14" // #55 (- 48) = 7
            },
            { 
              "label": "AM/PM", 
              "value": "1" // #49 (- 48) = 1
            },
            { 
              "label": "Seconds", 
              "value": "2" // #50 (- 48) = 2
            },
            { 
              "label": "Alternative Timezone", 
              "value": "8" // #55 (- 48) = 7
            },
            { 
              "label": "Custom Text", 
              "value": "3" // #51 (- 48) = 3
            },
            { 
              "label": "Time Active", 
              "value": "16" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned at rest", 
              "value": "17" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned active", 
              "value": "18" // #52 (- 48) = 4
            },
            { 
              "label": "Step Count", 
              "value": "4" // #52 (- 48) = 4
            },
            { 
              "label": "Distance meters", 
              "value": "5" // #53 (- 48) = 5
            },
            { 
              "label": "Distance km", 
              "value": "6" // #54 (- 48) = 6
            },
            { 
              "label": "Distance miles", 
              "value": "7" // #55 (- 48) = 7
            }
            
          ]
        },
       {
          "type": "select",
          "id": "SECONDARY_INFO_2",   
          "messageKey": "SECONDARY_INFO_2",
          "defaultValue": "9",
          "label": "Option 2",
          "options": [
            { 
              "label": "Nothing", 
              "value": "9" // #55 (- 48) = 7
            },
            { 
              "label": "Local Time", 
              "value": "10" // #55 (- 48) = 7
            },
           
            { 
              "label": "Location", 
              "value": "0" // #48 (- 48) = 0 
            },
            { 
              "label": "Week Number", 
              "value": "11" // #49 (- 48) = 1
            },
            { 
              "label": "Day of the Week", 
              "value": "14" // #55 (- 48) = 7
            },
            { 
              "label": "AM/PM", 
              "value": "1" // #49 (- 48) = 1
            },
            { 
              "label": "Seconds", 
              "value": "2" // #50 (- 48) = 2
            },
            { 
              "label": "Alternative Timezone", 
              "value": "8" // #55 (- 48) = 7
            },
            { 
              "label": "Custom Text", 
              "value": "3" // #51 (- 48) = 3
            },
            { 
              "label": "Time Active", 
              "value": "16" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned at rest", 
              "value": "17" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned active", 
              "value": "18" // #52 (- 48) = 4
            },
            { 
              "label": "Step Count", 
              "value": "4" // #52 (- 48) = 4
            },
            { 
              "label": "Distance meters", 
              "value": "5" // #53 (- 48) = 5
            },
            { 
              "label": "Distance km", 
              "value": "6" // #54 (- 48) = 6
            },
            { 
              "label": "Distance miles", 
              "value": "7" // #55 (- 48) = 7
            }
            
          ]
        },
      {
          "type": "select",
          "id": "SECONDARY_INFO_3",   
          "messageKey": "SECONDARY_INFO_3",
          "defaultValue": "9",
          "label": "Option 3",
          "options": [
            { 
              "label": "Nothing", 
              "value": "9" // #55 (- 48) = 7
            },
             { 
              "label": "Local Time", 
              "value": "10" // #55 (- 48) = 7
            },
            { 
              "label": "Location", 
              "value": "0" // #48 (- 48) = 0 
            },
            { 
              "label": "Week Number", 
              "value": "11" // #49 (- 48) = 1
            },
            { 
              "label": "Day of the Week", 
              "value": "14" // #55 (- 48) = 7
            },
            { 
              "label": "AM/PM", 
              "value": "1" // #49 (- 48) = 1
            },
            { 
              "label": "Seconds", 
              "value": "2" // #50 (- 48) = 2
            },
            { 
              "label": "Alternative Timezone", 
              "value": "8" // #55 (- 48) = 7
            },
            { 
              "label": "Custom Text", 
              "value": "3" // #51 (- 48) = 3
            },
            { 
              "label": "Time Active", 
              "value": "16" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned at rest", 
              "value": "17" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned active", 
              "value": "18" // #52 (- 48) = 4
            },
            { 
              "label": "Step Count", 
              "value": "4" // #52 (- 48) = 4
            },
            { 
              "label": "Distance meters", 
              "value": "5" // #53 (- 48) = 5
            },
            { 
              "label": "Distance km", 
              "value": "6" // #54 (- 48) = 6
            },
            { 
              "label": "Distance miles", 
              "value": "7" // #55 (- 48) = 7
            }
            
          ]
        },
       {
          "type": "select",
          "id": "SECONDARY_INFO_4",   
          "messageKey": "SECONDARY_INFO_4",
          "defaultValue": "9",
          "label": "Option 4",
          "options": [
            { 
              "label": "Nothing", 
              "value": "9" // #55 (- 48) = 7
            },
            { 
              "label": "Local Time", 
              "value": "10" // #55 (- 48) = 7
            },
           
            { 
              "label": "Location", 
              "value": "0" // #48 (- 48) = 0 
            },
            { 
              "label": "Week Number", 
              "value": "11" // #49 (- 48) = 1
            },
            { 
              "label": "Day of the Week", 
              "value": "14" // #55 (- 48) = 7
            },
            { 
              "label": "AM/PM", 
              "value": "1" // #49 (- 48) = 1
            },
            { 
              "label": "Seconds", 
              "value": "2" // #50 (- 48) = 2
            },
            { 
              "label": "Alternative Timezone", 
              "value": "8" // #55 (- 48) = 7
            },
            { 
              "label": "Custom Text", 
              "value": "3" // #51 (- 48) = 3
            },
            { 
              "label": "Time Active", 
              "value": "16" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned at rest", 
              "value": "17" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned active", 
              "value": "18" // #52 (- 48) = 4
            },
            { 
              "label": "Step Count", 
              "value": "4" // #52 (- 48) = 4
            },
            { 
              "label": "Distance meters", 
              "value": "5" // #53 (- 48) = 5
            },
            { 
              "label": "Distance km", 
              "value": "6" // #54 (- 48) = 6
            },
            { 
              "label": "Distance miles", 
              "value": "7" // #55 (- 48) = 7
            }
            
          ]
        },
       {
          "type": "select",
          "id": "SECONDARY_INFO_5",   
          "messageKey": "SECONDARY_INFO_5",
          "defaultValue": "9",
          "label": "Option 5",
          "options": [
            { 
              "label": "Nothing", 
              "value": "9" // #55 (- 48) = 7
            },
           { 
              "label": "Local Time", 
              "value": "10" // #55 (- 48) = 7
            },
           
            { 
              "label": "Location", 
              "value": "0" // #48 (- 48) = 0 
            },
            { 
              "label": "Week Number", 
              "value": "11" // #49 (- 48) = 1
            },
            { 
              "label": "Day of the Week", 
              "value": "14" // #55 (- 48) = 7
            },
            { 
              "label": "AM/PM", 
              "value": "1" // #49 (- 48) = 1
            },
            { 
              "label": "Seconds", 
              "value": "2" // #50 (- 48) = 2
            },
            { 
              "label": "Alternative Timezone", 
              "value": "8" // #55 (- 48) = 7
            },
            { 
              "label": "Custom Text", 
              "value": "3" // #51 (- 48) = 3
            },
            { 
              "label": "Time Active", 
              "value": "16" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned at rest", 
              "value": "17" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned active", 
              "value": "18" // #52 (- 48) = 4
            },
            { 
              "label": "Step Count", 
              "value": "4" // #52 (- 48) = 4
            },
            { 
              "label": "Distance meters", 
              "value": "5" // #53 (- 48) = 5
            },
            { 
              "label": "Distance km", 
              "value": "6" // #54 (- 48) = 6
            },
            { 
              "label": "Distance miles", 
              "value": "7" // #55 (- 48) = 7
            }
            
          ]
        },
       {
          "type": "select",
          "id": "SECONDARY_INFO_6",   
          "messageKey": "SECONDARY_INFO_6",
          "defaultValue": "9",
          "label": "Option 6",
          "options": [
            { 
              "label": "Nothing", 
              "value": "9" // #55 (- 48) = 7
            },
            { 
              "label": "Location", 
              "value": "0" // #48 (- 48) = 0 
            },
            { 
              "label": "Week Number", 
              "value": "11" // #49 (- 48) = 1
            },
            { 
              "label": "Day of the Week", 
              "value": "14" // #55 (- 48) = 7
            },
            { 
              "label": "Local Time", 
              "value": "10" // #55 (- 48) = 7
            },
           
            { 
              "label": "AM/PM", 
              "value": "1" // #49 (- 48) = 1
            },
            { 
              "label": "Seconds", 
              "value": "2" // #50 (- 48) = 2
            },
            { 
              "label": "Alternative Timezone", 
              "value": "8" // #55 (- 48) = 7
            },
            { 
              "label": "Custom Text", 
              "value": "3" // #51 (- 48) = 3
            },
            { 
              "label": "Time Active", 
              "value": "16" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned at rest", 
              "value": "17" // #52 (- 48) = 4
            },
            { 
              "label": "Calories burned active", 
              "value": "18" // #52 (- 48) = 4
            },
            { 
              "label": "Step Count", 
              "value": "4" // #52 (- 48) = 4
            },
            { 
              "label": "Distance meters", 
              "value": "5" // #53 (- 48) = 5
            },
            { 
              "label": "Distance km", 
              "value": "6" // #54 (- 48) = 6
            },
            { 
              "label": "Distance miles", 
              "value": "7" // #55 (- 48) = 7
            }
            
          ]
        },
      
        {
            "type": "input",                    
            "messageKey": "RANDOM_TEXT",
            "id":  "RANDOM_TEXT",
            "defaultValue": "",
            "label": "Custom Text",
         },
      
      {
          "type": "select",
          "id": "ALT_TIMEZONE_NAME",   
          "messageKey": "ALT_TIMEZONE_NAME",
          "defaultValue": "NYC",
          "label": "Alternative Timezone",
          "options": [
              { 
                "label": "Beijing Time", 
                "value": "BJS"  
              },
              { 
                "label": "Brussels Time", 
                "value": "BRU"  
              },
              { 
                "label": "Buenos Aires Time", 
                "value": "BUE"  
              },
              { 
                "label": "Cairo Time", 
                "value": "CAI"  
              },
              { 
                "label": "GMT Time", 
                "value": "GMT"  
              },
              { 
                "label": "Jakarta Time", 
                "value": "JKT"  
              },
              { 
                "label": "Hong Kong Time", 
                "value": "HKG"  
              },
              { 
                "label": "Kiev Tinme", 
                "value": "KIE"  
              },
              { 
                "label": "London Time", 
                "value": "LON"  
              },
              { 
                "label": "Los Angeles Time", 
                "value": "LAX"  
              },
              { 
                "label": "Madrid Time", 
                "value": "MAD"  
              },
              { 
                "label": "Melbourne Time", 
                "value": "MEL"  
              },
              { 
                "label": "Mexico City Time", 
                "value": "MEX"  
              },
              { 
                "label": "Moscow Time", 
                "value": "MOW"  
              },
              { 
                "label": "New Delhi Time", 
                "value": "DEL"  
              },
              { 
                "label": "New York Time", 
                "value": "NYC"  
              },
              { 
                "label": "New Zealand Time", 
                "value": "NZD"  
              },
              { 
                "label": "Paris Time", 
                "value": "PAR"  
              },
              { 
                "label": "Rio de Janeiro Time", 
                "value": "RIO"  
              },            
              { 
                "label": "Rome Time", 
                "value": "ROM"  
              },
              { 
                "label": "Salt Lake City Time", 
                "value": "SLC"  
              },
              { 
                "label": "Sydney Time", 
                "value": "SYD"  
              },
              { 
                "label": "Tokyo Time", 
                "value": "TYO"  
              },
              { 
                "label": "Washington Time", 
                "value": "DCA"  
              }
          ]
      },
      
      {
            "type": "input",                    
            "messageKey": "ALT_TIMEZONE_OFFSET",
            "id":  "ALT_TIMEZONE_OFFSET",
            "defaultValue": "0",
            "label": "Alternative Timezone offset (hidden)",
         }
  
    ]},
  
        
  
  
  //*********************** Behavior ***********************************
  
  {
    "type": "section",
    "items": [
    {
      "type": "heading",
      "defaultValue": "Behavior"
    },
      
      {
          "type": "select",
          "id": "LANGUAGE",       
          "messageKey": "LANGUAGE",
          "defaultValue": "0",
          "label": "Language",
          "capabilities":  ["NOT_PLATFORM_APLITE"],
          "options": [
            { 
              "label": "System Default", 
              "value": "0"
            },
            { 
              "label": "Català", 
              "value": "4" 
            },
             { 
              "label": "Magyar", 
              "value": "2" 
            },
                     { 
              "label": "Nederlandse", 
              "value": "5" 
            },
            { 
              "label": "Norsk", 
              "value": "1" 
            },
            { 
              "label": "Svenska", 
              "value": "3" 
            },
          ]
        },
      
    
      {
      "type": "select",
      "id": "BLUETOOTH_ALERT",        // Bluetooth
      "messageKey": "BLUETOOTH_ALERT",
      "defaultValue": "2",
      "label": "Bluetooth Alert",
      "options": [
        { 
          "label": "Silent",
          "value": "1" 
        },
        { 
          "label": "Weak",
          "value": "2" 
        },
        { 
          "label": "Normal",
          "value": "3" 
        },
        { 
          "label": "Strong",
          "value": "4" 
        },
        { 
          "label": "Double",
          "value": "5" 
        }
      ]
    },
      
     {
       "type": "toggle",
       "id": "BACKLIGHT_WHILE_CHARGING", 
       "messageKey": "BACKLIGHT_WHILE_CHARGING",
       "defaultValue": 0,
       "label": "Backlight while charging"
     },
   ]
      
  },
  
  
  
   //***************** WEATHER *****************
 {     
  "type": "section",
  "items": [
    {
      "type": "heading",
      "id": "weatherHeading",
      "defaultValue": "Weather"
    },
    
     {
      "type": "select",
      "id": "WEATHER_PROVIDER",        //Weather provider
      "messageKey": "WEATHER_PROVIDER",
      "defaultValue": "0",
      "label": "Weather Provider",
      "options": [
        { 
          "label": "Open Weather Map", 
          "value": "0" // #48 (- 48) = 0 
        },
        { 
          "label": "Weather Underground", 
          "value": "1" // #49 (- 48) = 1
        },
        { 
          "label": "Forecast.IO",
          "value": "2" // #50 (- 48) = 2
        }
      ]
    },
    
    {
      "type": "input",                    
      "messageKey": "FORECAST_API_KEY",
      "id":  "FORECAST_API_KEY",
      "defaultValue": "",
      "label": "Forecast.io API KEY",
   },
    
     {
      "type": "input",                    
      "messageKey": "OWM_API_KEY",
      "id":  "OWM_API_KEY",
      "defaultValue": "",
      "label": "Open Weather Map API KEY",
    },
    
     {
      "type": "input",                    
      "messageKey": "WU_API_KEY",
      "id":  "WU_API_KEY",
      "defaultValue": "",
      "label": "Weather Underground API KEY",
       "description":"You can retrieve your keys stored at https://www.pmkey.xyz by filling the form below"
    },
    
  
      
    {
      "type": "input",
      "id":  "MASTER_KEY_EMAIL", 
      "defaultValue": "",
      "label": "Master Key email",
      "attributes": {"type": "email"}
    },  
    
    {
      "type": "input",
      "id":  "MASTER_KEY_PIN", 
      "defaultValue": "",
      "label": "Master Key pin",
      "attributes": {"type": "password"}
    },  
      
      
      {
        "type": "button",
        "id":  "MASTER_KEY_BUTTON",
        "defaultValue": "GET KEYS"
      },
    
    
    
   {
      "type": "select",
      "id": "WEATHER_INTERVAL",        //Weather update interval
      "messageKey": "WEATHER_INTERVAL",
      "defaultValue": "60",
      "label": "Update Interval",
      "options": [
        { 
          "label": "15 min", 
          "value": "15" // #35 (- 20) = 15 
        },
        { 
          "label": "30 min", 
          "value": "30" // #50 (- 20) =  30
        },
        { 
          "label": "1 hour",
          "value": "60" // #80 (- 20) = 60
        }
      ]
    },
      
    {
      "type": "select",
      "id": "TEMPERATURE_FORMAT",        //Temperature format
      "messageKey": "TEMPERATURE_FORMAT",
      "defaultValue": "2",
      "label": "Temperature format",
      "options": [
        { 
          "label": "Kelvin", 
          "value": "0" // #48 (- 48) = 0 
        },
        { 
          "label": "Celsius", 
          "value": "1" // #48 (- 48) = 0 
        },
        { 
          "label": "Fahrenheit", 
          "value": "2" // #48 (- 48) = 0 
        }
      ]
    },
      
    {
      "type": "select",
      "id": "LOCATION_SERVICE",        //weather location: automatic or manual
      "messageKey": "LOCATION_SERVICE",
      "defaultValue": "0",
      "label": "Location",
      "options": [
        { 
          "label": "Automatic", 
          "value": "0" // #48 (- 48) = 0 
        },
        { 
          "label": "Manual", 
          "value": "1" // #48 (- 48) = 0 
        }
      ]
    },  
    
    {
      "type": "input",
      "messageKey": "LOCATION_LATITUDE",
      "id":  "LOCATION_LATITUDE",
      "defaultValue": "",
      "label": "Location coordinates: Latitude",
    },
  
    {
      "type": "input",
      "messageKey": "LOCATION_LONGITUDE",
      "id":  "LOCATION_LONGITUDE",
      "defaultValue": "",
      "label": "Location coordinates: Longitude",
      "description": "You can lookup coordinates using form below"
    },
      
    {
      "type": "text",
      "id":  "LOCATION_RESULT", 
      "defaultValue": "",
    },
      
    {
      "type": "input",
      "id":  "LOCATION_NAME", 
      "defaultValue": "",
      "label": "Location Name",
      "description": "Enter town or city name"
    },  
      
      
      {
        "type": "button",
        "id":  "LOCATION_BUTTON",
        "defaultValue": "Coordinates Lookup"
      }


      ]
    },
  
  
  {
  "type": "submit",
  "defaultValue": "Save"
}  
  
  
];