var ultra_sonic = document.getElementById('wrapper');
var ultra_text = document.getElementById('ultra_text');


function update_ultrasonic(data){
	data = data * 100; // m to cm
	if(data > 58)
		data = 58;
	if(data < 0)
		data = 0;
	
	data = Math.round(data);

	ultra_text.textContent = data.toString();

	if(data < 15) {
		ultra_sonic.setAttribute('fill', '#ff0000');
	}
	else {
		ultra_sonic.setAttribute('fill', '#00ff00');
	}
}
