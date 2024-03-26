#pragma once

#ifdef PBRT_FLOAT_AS_DOUBLE
	using Float = double;
#else
	using Float = float;
#endif