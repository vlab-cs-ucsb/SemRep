function reference_function($x){ 
	if (strlen($x) > 4)
		exit(); 
	else {
		$x = preg_replace('/</', '', $x); 
		if ($x == '')
			exit(); 
		else
			return $x;
	}
}