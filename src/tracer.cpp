
#include "common.h"

/*---------------------------------------------------------------------------*/

#define EPSILON			0.0001

/*---------------------------------------------------------------------------*/

Entity *RayTracer::FindNearest (const Ray &ray, double &distance, int &result, bool skip_lights) const {
	Entity *entity = NULL;
	result = 1;
	for (EntityList::const_iterator iter = scene->First (), end = scene->Last (); iter != end; ++iter) {
		Entity *curr = (*iter);
		if (skip_lights && dynamic_cast<Light *> (curr)) {
			continue;
		}
		if (int _result  = curr->Intersection (ray, distance)) {
			entity = curr;
			result = _result;
		}
	}
	return entity;
}

/*---------------------------------------------------------------------------*/

Entity *RayTracer::Trace (const Ray &ray, Color &color, int depth, double refraction_index, double &distance) const {
	if (depth > max_depth) {
		return NULL;
	}

	distance = std::numeric_limits<double>::infinity ();
	
	int result = 1;
	Entity *entity = FindNearest (ray, distance, result);

	if (entity == NULL) {
		// Failed to intersect with an object
		return entity;
	}

	if (Light *light = dynamic_cast<Light *> (entity)) {
		// Intersected with a light
		color = light->GetMaterial ()->Emission ();
	} else {
		// Intersected with an object in the scene
		Vector p = ray.PointAt (distance);
		
		// Trace the ray to the light
		for (LightList::const_iterator iter = scene->FirstLight(), end = scene->LastLight (); iter != end; ++iter) {
			Light *light = (*iter);
			double shade = 1.0;
			if (light->GetType () == Light::POINT) {
				// Handle point light shadows
				Vector L = light->Position () - p;
				double tdist = L.Magnitude ();
				L *= (1.0 / tdist);
				int temp_result = 1;
				Entity *temp = FindNearest (Ray (p + L * EPSILON, L), tdist, temp_result, false);
				if (temp != light) {
					shade = 0.0;
				}
			} else if (light->GetType () == Light::DIRECTIONAL) {
				// Handle directional light shadows
				shade = 0.0;
				Vector L = ((light->Position () + (0.5 * light->Radius ())) - p).Normal ();
				const double inverse_grid_size_squared = 1.0 / (double) (grid_size * grid_size);
				for (int i = 0; i < grid_size; ++i) {
					for (int j = 0; j < grid_size; ++j) {
						Vector LP = Vector (light->Position ()[0] + (double) i, light->Position ()[1], light->Position ()[2] + (double) j);
						Vector direction = LP - p;
						double ldist = direction.Magnitude ();
						direction *= (1.0 / ldist);
						int temp_result = 1;
						Entity *temp = FindNearest (Ray (p + direction * EPSILON, direction), ldist, temp_result, false);
						if (temp == light) {
							shade += inverse_grid_size_squared;
						}
					}
				}
			} else if (light->GetType () == Light::SPOT) {
				// TODO: Compute spot light shadows
			}

			// Calculate diffuse lighting
			Vector L = (light->Position () - p).Normal ();
			Vector N = entity->NormalAt (p);
			if (entity->GetMaterial ()->GetProperties () & Material::DIFFUSE) {
				double dot = L.Dot (N);
				if (dot > 0.0) {
					Color diff = dot * entity->GetMaterial ()->Diffuse () * shade;
					color += diff * light->GetMaterial ()->Emission () * entity->TexelAt (p);
				}
			}

			// Calculate specular lighting
			if (entity->GetMaterial ()->GetProperties () & Material::SPECULAR) {
				Vector V = ray.Direction ();
				Vector R = L - 2.0 * L.Dot (N) * N;
				double dot = V.Dot (R);
				if (dot > 0.0) {
					Color spec = pow (dot, entity->GetMaterial ()->Shininess ()) * entity->GetMaterial ()->Specular () * shade;
					color += spec * light->GetMaterial()->Emission ();
				}
			}
		}

		// Trace reflection rays
		double refl = entity->GetMaterial()->Reflectivity ();
		if (refl > 0.0) {
			double diffuse_refl = entity->GetMaterial ()->DiffuseReflectivity ();
			if (diffuse_refl > 0.0 && depth < max_reflection_depth) {
				// Trace diffuse reflections
				Vector N = entity->NormalAt (p);
				Vector RP = ray.Direction () - 2.0 * ray.Direction ().Dot (N) * N;
				Vector RN1 = Vector (RP[2], RP[1], -RP[0]);
				Vector RN2 = RP.Cross (RN1);
				refl *= 1.0; // scale;
				// TODO: Perform Stochastic sampling here
				for (int i = 0; i < max_samples; ++i) {
					Vector R = (RP + RN1 * 1.0 + RN2 * 1.0 * diffuse_refl).Normal ();
					Color reflection_color;
					double reflection_distance;
					Trace (Ray (p + R * EPSILON, R), reflection_color, depth + 1, refraction_index, reflection_distance);
					color += refl * reflection_color * entity->TexelAt (p);
				}
			} else {
				// Trace normal reflections
				Vector N = entity->NormalAt (p);
				Vector R = ray.Direction () - 2.0 * ray.Direction ().Dot (N) * N;
				if (depth < max_depth) {
					Color reflection_color;
					double reflection_distance;
					Trace (Ray (p + R * EPSILON, R), reflection_color, depth + 1, refraction_index, reflection_distance);
					color += refl * reflection_color * entity->TexelAt (p);
				}
			}
		}

		// Trace refracted rays
		double refraction = entity->GetMaterial ()->Refractiveness ();
		if (refraction > 0.0) {
			double rindex = entity->GetMaterial ()->RefractionIndex ();
			double n = refraction_index / rindex;
			Vector N = entity->NormalAt (p) * (double) result;
			double cosi = -N.Dot (ray.Direction ());
			double cost = 1.0 - n * n * (1.0 - cosi * cosi);
			if (cost > 0.0) {
				Vector T = (n * ray.Direction ()) + (n * cosi - sqrt (cost)) * N;
				Color refraction_color;
				double refraction_distance;
				Trace (Ray (p + T * EPSILON, T), refraction_color, depth + 1, rindex, refraction_distance);

				Color absorbance = entity->TexelAt (p) * entity->GetMaterial ()->Density () * -refraction_distance;
				Color transparency = Color (exp (absorbance[0]), exp (absorbance[1]), exp (absorbance[2]));
				color += refraction_color * transparency;
			}
		}
	}
	
	return entity;
}

static Random tracer_random (65535UL);

double Percentage () {
	return 0.01 * (tracer_random.RandomInt (100) - 50.0);
}

void RayTracer::Render (unsigned int* dest) const {
	Entity *entity = NULL;
	double sy = camera->Top ();
	double dx = camera->DeltaX ();
	double dy = camera->DeltaY ();
	double fifth_dx = 0.2 * dx;
	double fifth_dy = 0.2 * dy;
	size_t index = 0;

	Entity **last_row = new (GC) Entity *[camera->ScreenWidth ()];
	memset (last_row, 0, camera->ScreenWidth () * 4);
	Entity *last = NULL;

	for (size_t y = 0; y < camera->ScreenHeight (); ++y) {
		double sx = camera->Left ();
		for (size_t x = 0; x < camera->ScreenWidth (); ++x) {
			Color color (0.0, 0.0, 0.0);
			Vector dir = (Vector (sx, sy, 0.0) - camera->Origin ()).Normal ();
			Ray ray (camera->Origin (), dir);
			double dist;
			entity = Trace (ray, color, 1, 1.0, dist);
			// Perform Stochastic sampling here
			double _sx = sx;
			for (size_t i = -2; i <= 2; ++i) {
				double _sy = sy;
				for (size_t j = -2; j <= 2; ++j) {
					Color sub_color;
					Vector sub_dir = (Vector (_sx + (fifth_dx * Percentage ()), _sy + (fifth_dy * Percentage ()), 0.0) - camera->Origin ()).Normal ();
					Ray sub_ray = Ray (camera->Origin (), sub_dir);
					double sub_dist;
					Trace (sub_ray, sub_color, 1, 1.0, sub_dist);
					color.BlendLerp (sub_color, 0.04);
					_sy += fifth_dy;
				}
				_sx += fifth_dx;
			}
			if (last != entity || last_row[x] != entity) {
				// Perform Adaptive Supersampling (Horizontal and Vertical Edge)
				Color sub_color (0.0, 0.0, 0.0);
				_sx = sx;
				for (size_t i = -2; i <= 2; ++i) {
					double _sy = sx;
					for (size_t j = -2; j <= 2; ++j) {
						Vector sub_dir = (Vector (_sx, _sy, 0.0) - camera->Origin ()).Normal ();
						Ray sub_ray = Ray (camera->Origin (), sub_dir);
						double sub_dist;
						Trace (sub_ray, sub_color, 1, 1.0, sub_dist);
						_sy += fifth_dy;
					}
					_sx += fifth_dx;
				}
				color = sub_color * 0.04;
			}
			last_row[x] = entity;
			last = entity;
			
			// Apply gamma correction
			color = Color (pow (color[0], gamma), pow (color[1], gamma), pow (color[2], gamma));
			dest[index++] = (unsigned int) color;
			sx += dx;
		}
		sy += dy;
	}
}
