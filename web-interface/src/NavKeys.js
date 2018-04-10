/*!
 * NavKeys.js
 * Version 1.0
 *
 * Used to mark(grey) pressed navigation keys in web-interface
 */

//ASCII codes for arrow keys
var key_left = 37;
var key_forward = 38;
var key_right = 39;

//ASCII codes for A W D keys
var key_A_left = 65;
var key_W_forward = 87;
var key_D_right = 68;

function initializeNavKeys() {

    var keyState = [false, false, false];


    //when arrow key is pressed down
    $(document).keydown(function (e) {
        if (e.keyCode == key_left || e.keyCode == key_A_left){
            
            keyState[0] = true;
            document.getElementById("LeftBtn").style.backgroundColor = "gray";
            c.left(); 

        }
        if (e.keyCode == key_forward || e.keyCode == key_W_forward){
            
            keyState[1] = true;
            document.getElementById("ForwardBtn").style.backgroundColor = "gray";
        }
        if (e.keyCode == key_right || e.keyCode == key_D_right){
            
            keyState[2] = true;
            document.getElementById("RightBtn").style.backgroundColor = "gray";
        }
    });

    //when arrow key is released
    $(document).keyup(function (e) {
        if (e.keyCode == key_left || e.keyCode == key_A_left){
            keyState[0] = false;
            document.getElementById("LeftBtn").style.backgroundColor = "";
        }
        if (e.keyCode == key_forward || e.keyCode == key_W_forward){
            keyState[1] = false;
            document.getElementById("ForwardBtn").style.backgroundColor = "";
        }
        if (e.keyCode == key_right || e.keyCode == key_D_right){
            keyState[2] = false;
            document.getElementById("RightBtn").style.backgroundColor = "";
        }
    });
}


function sendTurnRight(){
      var message = 15;
      console.log("message: ", message);
      var xhr = new XMLHttpRequest();
      xhr.open("POST", '/', true);
      xhr.responseType = "arraybuffer";

      xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");

      xhr.onreadystatechange = function() {
          if(xhr.readyState == 4 && xhr.status == 200) {
              console.log("xhr");
          }
      }
      xhr.send(message);
}

function sendTurnLeft(){
      var message = -15;
      console.log("message: ", message);
      var xhr = new XMLHttpRequest();
      xhr.open("POST", '/', true);
      xhr.responseType = "arraybuffer";

      xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");

      xhr.onreadystatechange = function() {
          if(xhr.readyState == 4 && xhr.status == 200) {
              console.log("xhr");
          }
      }
      xhr.send(message);
}

function sendMoveForward(){

    var message = 
    {
        "dataType": 1045,
        "payload": {
            "fields": [
                {
                    "name": "percent",
                    "type": "number",
                    "value": 35
                }  
            ],
            "name": "opendlv.proxy.PedalPositionReading"
        }
    };

    var c = Module.ccall('forward', 
        'void',
        ['void'],
        []
      );
    //ws.send(JSON.stringify(message))
        /*
      console.log(message);
      var xhr = new XMLHttpRequest();
      xhr.open("POST", '/', true);
      xhr.responseType = "arraybuffer";

      xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");

      xhr.onreadystatechange = function() {
          if(xhr.readyState == 4 && xhr.status == 200) {
              
          }
      }
      xhr.send(message);
      */
}

/*
{
    dataType: [id]
    payload: {
        fields: [
            {
                name: [name],
                type: [type],
                value: [value]
            }
        ],
        name: [name of data type]
    }
}
*/

