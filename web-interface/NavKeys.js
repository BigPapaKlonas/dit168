

//ASCII codes
var key_left = 37;
var key_forward = 38;
var key_right = 39;

function initializeNavKeys() {

    var keyState = [false, false, false];

    //if (any) arrow key is pressed down
    $(document).keydown(function (e) {
        if (e.keyCode == key_left){
            keyState[0] = true;
        }
        if (e.keyCode == key_forward){
            keyState[1] = true;
            document.getElementById("ForwardBtn").style.backgroundColor = "gray";
        }
        if (e.keyCode == key_right){
            keyState[2] = true;
        }
    });

    $(document).keyup(function (e) {
        if (e.keyCode == key_left){
            keyState[0] = false;
        }
        if (e.keyCode == key_forward){
            keyState[1] = false;
            document.getElementById("ForwardBtn").style.backgroundColor = "";
        }
        if (e.keyCode == key_right){
            keyState[2] = false;
        }
        
    });
}

