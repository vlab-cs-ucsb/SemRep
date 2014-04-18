function reference_function(x){ 
	if (x.length > 4)
		return; 
	else {
		x.replace(/</, ""); 
		if (x === ’’)
			return; 
		else
			return x;
	}
}