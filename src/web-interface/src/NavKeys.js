/*!
 * NavKeys.js
 * Version 2.0
 *
 * Used to mark(grey) pressed navigation keys and call their functions in web-interface
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


    //when arrow key is pressed down and keyState is false (keyState used to not flood with messages)
    $(document).keydown(function (e) {
        if ((e.keyCode == key_left || e.keyCode == key_A_left) && !keyState[0]){
            
            keyState[0] = true;
            document.getElementById("LeftBtn").style.backgroundColor = "gray";
            turn("left");
        }
        if ((e.keyCode == key_forward || e.keyCode == key_W_forward ) && !keyState[1]){
            keyState[1] = true;
            
            document.getElementById("ForwardBtn").style.backgroundColor = "gray";
            move("forward");
        }
        if ((e.keyCode == key_right || e.keyCode == key_D_right) && !keyState[2]){
            
            keyState[2] = true;
            document.getElementById("RightBtn").style.backgroundColor = "gray";
            turn("right");
        }
    });

    //when arrow key is released
    $(document).keyup(function (e) {
        if (e.keyCode == key_left || e.keyCode == key_A_left){
            keyState[0] = false;
            document.getElementById("LeftBtn").style.backgroundColor = "";
            turn("straight");
        }
        if (e.keyCode == key_forward || e.keyCode == key_W_forward){
            keyState[1] = false;
            document.getElementById("ForwardBtn").style.backgroundColor = "";
            move("stop");
        }
        if (e.keyCode == key_right || e.keyCode == key_D_right){
            keyState[2] = false;
            document.getElementById("RightBtn").style.backgroundColor = "";
            turn("straight");
        }
    });
}