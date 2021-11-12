#pragma once

typedef enum {
	S,  // shift           (<)
	R,  // reduce          (>)
	E,  // equal           (=)
	X   // nothing - error ( )
}ExprAction;

