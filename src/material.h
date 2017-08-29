#ifndef __MATERIAL_H_
#define __MATERIAL_H_

#pragma once
/*---------------------------------------------------------------------------*/

class Material : public gc_cleanup {
public:
	enum Properties {
		NONE		= 0,
		AMBIENT		= 0x0001,
		DIFFUSE		= 0x0002,
		SPECULAR	= 0x0004,
		EMISSION	= 0x0008,
		REFLECTIVE	= 0x0010,
		REFRACTIVE	= 0x0020,
		TEXTURE		= 0x8000,

		ALL			= 0x803F,
	};

private:
	int properties;
	Texture *texture;
	Color ambient;
	Color diffuse;
	Color specular;
	double shininess;
	Color emission;
	double reflectivity;
	double diffuse_reflectivity;
	double refractiveness;
	double refractionIndex;
	double density;

public:
	inline Material () : properties (ALL), texture (NULL), ambient (Color::WHITE), diffuse (Color::WHITE), specular (), shininess (20.0), emission (), reflectivity (0.0), diffuse_reflectivity (0.0), refractiveness (0.0), refractionIndex (0.0), density (0.15) { }
	virtual ~Material () { }

	inline int GetProperties () const {
		return properties;
	}
	inline int SetProperties (int _properties) {
		return properties = _properties;
	}
	
	inline Texture *GetTexture () const {
		return texture;
	}
	inline void SetTexture (Texture *_texture) {
		texture = _texture;
	}

	inline Color Ambient () const {
		return ambient;
	}
	inline Color &Ambient () {
		return ambient;
	}

	inline Color Diffuse () const {
		return diffuse;
	}
	inline Color &Diffuse () {
		return diffuse;
	}

	inline Color Specular () const {
		return specular;
	}
	inline Color &Specular () {
		return specular;
	}
	
	inline double Shininess () const {
		return shininess;
	}
	inline double &Shininess () {
		return shininess;
	}

	inline Color Emission () const {
		return emission;
	}
	inline Color &Emission () {
		return emission;
	}

	inline double Reflectivity () const {
		return reflectivity;
	}
	inline double &Reflectivity () {
		return reflectivity;
	}
	
	inline double DiffuseReflectivity () const {
		return diffuse_reflectivity;
	}
	inline double &DiffuseReflectivity () {
		return diffuse_reflectivity;
	}

	inline double Refractiveness () const {
		return refractiveness;
	}
	inline double &Refractiveness () {
		return refractiveness;
	}

	inline double RefractionIndex () const {
		return refractionIndex;
	}
	inline double &RefractionIndex () {
		return refractionIndex;
	}

	inline double Density () const {
		return density;
	}
	inline double &Density () {
		return density;
	}

}; /* class Material */

/*---------------------------------------------------------------------------*/
#endif /* __MATERIAL_H_ */
