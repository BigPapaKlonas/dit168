var ultra_sonic = document.getElementById('wrapper');
var ultra_text = document.getElementById('ultra_text');


function update_ultrasonic(data){
	if(data < 15){
		ultra_sonic.setAttribute('fill', '#ff0000');
		ultra_text.textContent = data.toString();
	}
}
