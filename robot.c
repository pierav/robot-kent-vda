
#define threshold_black 125
#define is_black(captor)       ((captor) < threshold_black ? (1) : (0))


unsigned char right,left;

//loop ->

//reading
/**
*	White ========= Gray ======== Black
*	  0				 75            125
*/

a_right = (unsigned char)(analogRead(AN3)/4);
a_left = (unsigned char)(analogRead(AN0)/4);

/**
*	White ======================= Black
*	  0	  			                1
*/
d_r = (unsigned char)(analogRead(D0));
d_l = (unsigned char)(analogRead(d1));

//...

// line tracking
//if the right sensor see white and the left see black, the robot turn left
if (!is_black(right) && is_black(left)) {
	turn_left	//Is macro better ?
}
//if the left sensor see white and the right see black, the robot turn right
else if (is_black(right) && !is_black(left)) {
	turn_right
}
//if both sensors see black, the robot goes straightan
else if (is_black(right) && is_black(left)) {
	straightan
}


// terms

if(d_r&&d_l)goto end;//good ?

//...

//end loop