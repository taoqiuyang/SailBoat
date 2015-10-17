<?php
	include 'PhpSerial.php';

	$serial = new PhpSerial;

	$serial->deviceSet("COM1");

	// Set for 9600-8-N-1 (no flow control)
	$serial->confBaudRate(9600); //Baud rate: 9600
	$serial->confParity("none");  //Parity (this is the "N" in "8-N-1")
	$serial->confCharacterLength(8); //Character length     (this is the "8" in "8-N-1")
	$serial->confStopBits(1);  //Stop bits (this is the "1" in "8-N-1")
	$serial->confFlowControl("none");

	$set = 0;
	
	// Then we need to open it
	$serial->deviceOpen();
	echo "Opening serial port <br/>";
	
	// To write into
	$serial->sendMessage("Hello !");

	// Or to read from
	$read = $serial->readPort();
	


	while($set == 0) {
		if(!empty($read)){
			$set = 1;
			echo "value is $read";	
		}else {
			echo "empty <br/>";
		}
	}

	echo "Finish reading";
	// Read data

	$serial->deviceClose();

?>
