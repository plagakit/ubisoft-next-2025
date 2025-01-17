#pragma once

#include "../external/next-api/App/AppSettings.h"

#ifndef PLATFORM_WINDOWS
#error Only Windows is supported by the NextAPI!
#endif

// Downscale the resolution of the depth buffer rasterizer when debug
// is on to improve the speed of the debug build - the release build 
// is much more optimized and doesn't need downscaling to run at a fast framerate
#define USE_DEBUG_RASTER_DOWNSCALING
#ifdef USE_DEBUG_RASTER_DOWNSCALING
	#ifdef _DEBUG
		#define RASTER_DOWNSCALING 3
	#else
		#define RASTER_DOWNSCALING 1
	#endif
#else
	#define RASTER_DOWNSCALING 1
#endif

// Use the painter's algorithm along w/ App::DrawLine to rasterize
// primitives made out of lines (wireframes) instead of using the
// depth buffer rasterizer. Will speed up rendering for small amounts
// of wireframes, but not for large ones, since the depth buffer 
// rasterizer can efficiently handle big clumps of same-coloured
// pixels, but not sparse ones.
#define USE_PAINTERS_FOR_WIREFRAME

// Enables trivial parallelization i.e. C++17's <execution> library
// and the parallel execution policy for some time-hungry algorithms
// (no ECS though). In the future I would like to get multithreading
// working with an actual thread and job system type thing.
#define USE_TRIVIAL_PARALLELIZATION
