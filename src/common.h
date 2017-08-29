#ifndef __COMMON_H_
#define __COMMON_H_

#pragma once
/*---------------------------------------------------------------------------*/

#include "gc.h"

#include <cstdio>
#include <cassert>
#include <cmath>
#include <limits>

#include <vector>
#include <algorithm>

/*---------------------------------------------------------------------------*/

#define E			2.71828182845904523536
#define INV_E		0.36787944117144232159

#define PI			3.141592653897932384626433
#define TWO_PI		6.283185307795864769252866
#define HALF_PI		1.570796326948966192313216
#define QUARTER_PI	0.785398163474483096156608
#define INV_PI		0.31830988615256964852
#define INV_TWO_PI	0.15915494307628482426

#define DEGS_TO_RADS(ang)	((ang) * PI / 180.0)
#define RADS_TO_DEGS(ang)	((ang) * 180.0 / PI)

/*---------------------------------------------------------------------------*/

#include "str.h"
#include "date.h"
#include "logger.h"

#include "random.h"

#include "vector.h"
#include "matrix.h"
#include "quaternion.h"

#include "ray.h"

#include "color.h"
#include "canvas.h"
#include "noise.h"
#include "texture.h"
#include "procedural.h"
#include "material.h"

#include "bounding_box.h"

#include "entity.h"
#include "plane.h"
#include "sphere.h"
#include "torus.h"
#include "light.h"

#include "camera.h"

#include "scene.h"

#include "tracer.h"

/*---------------------------------------------------------------------------*/
#endif /* __COMMON_H_ */
