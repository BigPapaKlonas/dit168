function getMessage(){

	var oReq = new XMLHttpRequest();
	oReq.open("GET", "/message", true);
	oReq.responseType = "arraybuffer";

	oReq.onload = function (oEvent) {
	  var arrayBuffer = oReq.response; // Note: not oReq.responseText
	  if (arrayBuffer) {
	    var byteArray = new Uint8Array(arrayBuffer);
	    for (var i = 0; i < byteArray.byteLength; i++) {
	      console.log(String.fromCharCode(byteArray[i]));
	    }
	  }
	};

	oReq.send(null);

}