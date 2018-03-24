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

function initializeNavKeys() {

    var keyState = [false, false, false];

    //when arrow key is pressed down
    $(document).keydown(function (e) {
        if (e.keyCode == key_left){
            keyState[0] = true;
            document.getElementById("LeftBtn").style.backgroundColor = "gray";
        }
        if (e.keyCode == key_forward){
            keyState[1] = true;
            document.getElementById("ForwardBtn").style.backgroundColor = "gray";
        }
        if (e.keyCode == key_right){
            keyState[2] = true;
            document.getElementById("RightBtn").style.backgroundColor = "gray";
        }
    });

    //when arrow key is released
    $(document).keyup(function (e) {
        if (e.keyCode == key_left){
            keyState[0] = false;
            document.getElementById("LeftBtn").style.backgroundColor = "";
        }
        if (e.keyCode == key_forward){
            keyState[1] = false;
            document.getElementById("ForwardBtn").style.backgroundColor = "";
        }
        if (e.keyCode == key_right){
            keyState[2] = false;
            document.getElementById("RightBtn").style.backgroundColor = "";
        }
    });
}

