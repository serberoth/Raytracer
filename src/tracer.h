#ifndef __RAYTRACER_H_
#define __RAYTRACER_H_

#pragma once
/*---------------------------------------------------------------------------*/

class RayTracer : public gc_cleanup {
private:
	Camera *camera;
	Scene* scene;
	int max_depth;
	int grid_size;
	int max_reflection_depth;
	int max_samples;
	double gamma;

	Entity *FindNearest (const Ray &ray, double &distance, int &result, bool skip_lights = true) const;

	Entity *Trace (const Ray &ray, Color &color, int depth, double index, double &distance) const;

public:
	inline RayTracer () : camera (NULL), scene (NULL), max_depth (6), grid_size (4), max_reflection_depth (3), max_samples (1), gamma (1.0) { }
	virtual ~RayTracer () {
		camera = NULL;
		scene = NULL;
	}

	inline void Initialize (Camera *_camera, Scene *_scene) {
		assert (_camera != NULL);
		assert (_scene != NULL);
		camera = _camera;
		scene = _scene;
	}

	inline Camera *GetCamera () const {
		return camera;
	}

	inline Scene* GetScene () const {
		return scene;
	}

	inline int MaxDepth () const {
		return max_depth;
	}

	inline int GridSize () const {
		return grid_size;
	}

	inline int MaxReflectionDepth () const {
		return max_reflection_depth;
	}

	inline int MaxSamples () const {
		return max_samples;
	}

	inline double Gamma () const {
		return gamma;
	}

	void Render (unsigned int* dest) const;

};

/*---------------------------------------------------------------------------*/
#endif /* __RAYTRACER_H_ */
