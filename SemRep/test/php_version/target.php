function target_function($x){
	$x = preg_replace('/"/', '\"', $x); 
	return $x;
}