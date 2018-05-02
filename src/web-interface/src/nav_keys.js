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
var key_S_down = 83;

function initializeNavKeys() {

    var keyState = [false, false, false, false];


    //when arrow key is pressed down and keyState is false (keyState used to not flood with messages)
    $(document).keydown(function (e) {
        if( $("#left_btn").is(":enabled") ) {
            if (e.keyCode == key_A_left && !keyState[0]){
                
                keyState[0] = true;
                document.getElementById("left_btn").style.backgroundColor = "rgb(54, 80, 46)";
                turn("left");
            }
            if (e.keyCode == key_W_forward && !keyState[1]){
                keyState[1] = true;
                
                document.getElementById("forward_btn").style.backgroundColor = "rgb(54, 80, 46)";
                move("forward");
            }
            if (e.keyCode == key_D_right && !keyState[2]){
                
                keyState[2] = true;
                document.getElementById("right_btn").style.backgroundColor = "rgb(54, 80, 46)";
                turn("right");
            }
            if(e.keyCode == key_S_down && !keyState[3]){
                keyState[3] = true;
                
                document.getElementById("back_btn").style.backgroundColor = "rgb(54, 80, 46)";
                move("back");
            }
        }
    });

    //when arrow key is released
    $(document).keyup(function (e) {
        if( $("#left_btn").is(":enabled") ) {
            if (e.keyCode == key_A_left){
                keyState[0] = false;
                document.getElementById("left_btn").style.backgroundColor = "";
                turn("straight");
            }
            if (e.keyCode == key_W_forward){
                keyState[1] = false;
                document.getElementById("forward_btn").style.backgroundColor = "";
                move("stop");
            }
            if (e.keyCode == key_D_right){
                keyState[2] = false;
                document.getElementById("right_btn").style.backgroundColor = "";
                turn("straight");
            }
            if(e.keyCode == key_S_down){
                keyState[3] = false;
                
                document.getElementById("back_btn").style.backgroundColor = "";
                move("stop");
            }
        }
    });
}