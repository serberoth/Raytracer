
#include "common.h"
#include <sys/time.h>

/*---------------------------------------------------------------------------*/

// #define SCREEN_WIDTH	1024 // 1440
// #define SCREEN_HEIGHT	 768 //  960
#define SCREEN_WIDTH	1440
#define SCREEN_HEIGHT	 960

#define SIMPLE_SCENE	1

/*---------------------------------------------------------------------------*/

// extern void WriteTgaImage (char *filename, unsigned int *buffer, size_t width, size_t height);

/*---------------------------------------------------------------------------*/

Scene *CreateSimpleScene () {
	Scene *scene = new Scene ();
	Entity *entity = NULL;
	
	// Ground Plane
	entity = new Plane (Vector (0.0, 1.0, 0.0), 4.4);
	entity->GetMaterial ()->SetProperties (Material::TEXTURE | Material::AMBIENT | Material::DIFFUSE);
	Procedural *marble = new Marble ();
	marble->AddColor (-1.0, Color::WHITE);
	marble->AddColor (0.3, Color::DARK_GRAY);
	marble->AddColor (0.6, Color::BLACK);
	marble->AddColor (1.0, Color::WHITE);
	entity->GetMaterial ()->SetTexture (marble);
	// entity->GetMaterial ()->SetTexture (GenerateCheckerboard ());
	entity->GetMaterial ()->Ambient () = Color (0.5, 0.3, 0.3);
	entity->GetMaterial ()->Diffuse () = Color::WHITE;
	entity->GetMaterial ()->Specular () = Color::BLACK;
	entity->GetMaterial ()->Shininess () = 20.0;
	entity->GetMaterial ()->Reflectivity () = 0.0;
	entity->GetMaterial ()->DiffuseReflectivity () = 0.0;
	entity->GetMaterial ()->Refractiveness () = 0.0;
	entity->GetMaterial ()->RefractionIndex () = 1.5;
	entity->GetMaterial ()->Density () = 0.15;
	scene->Add (entity);
	
	// Ceiling Plane
	entity = new Plane (Vector (0.0, -1.0, 0.0), 10.0);
	entity->GetMaterial ()->SetProperties (Material::AMBIENT | Material::DIFFUSE | Material::SPECULAR | Material::REFLECTIVE);
	entity->GetMaterial ()->Ambient () = Color (0.45, 0.4, 0.6);
	entity->GetMaterial ()->Diffuse () = Color (0.25, 0.2, 0.4);
	entity->GetMaterial ()->Specular () = Color (0.8, 0.8, 0.8);
	entity->GetMaterial ()->Shininess () = 10.0;
	entity->GetMaterial ()->Reflectivity () = 0.1;
	entity->GetMaterial ()->DiffuseReflectivity () = 0.2;
	entity->GetMaterial ()->Refractiveness () = 0.0;
	entity->GetMaterial ()->RefractionIndex () = 1.5;
	entity->GetMaterial ()->Density () = 0.15;
	scene->Add (entity);
	
	// Large Sphere
	entity = new Sphere (Vector(1.0, -0.8, 3.0), 2.5);
	entity->GetMaterial ()->SetProperties (Material::TEXTURE | Material::AMBIENT | Material::DIFFUSE | Material::SPECULAR | Material::REFLECTIVE);
	entity->GetMaterial ()->SetTexture (new Checkerboard (Color::YELLOW, Color (0.6, 0.6, 0.8)));
	// entity->GetMaterial ()->SetTexture (GenerateCheckerboard (Color::YELLOW, Color (0.6, 0.6, 0.8)));
	entity->GetMaterial ()->Ambient () = Color (0.7, 0.7, 0.7);
	entity->GetMaterial ()->Diffuse () = Color (0.2, 0.2, 0.2);
	entity->GetMaterial ()->Specular () = Color (0.8, 0.8, 0.8);
	entity->GetMaterial ()->Shininess () = 20.0;
	entity->GetMaterial ()->Reflectivity () = 0.6;
	entity->GetMaterial ()->DiffuseReflectivity () = 0.4;
	entity->GetMaterial ()->Refractiveness () = 0.0;
	entity->GetMaterial ()->RefractionIndex () = 1.5;
	entity->GetMaterial ()->Density () = 0.15;
	scene->Add (entity);
	
	// Small Sphere
	entity = new Sphere (Vector (-5.5, -0.5, 7.0), 2.0);
	entity->GetMaterial ()->SetProperties (Material::AMBIENT | Material::DIFFUSE | Material::SPECULAR | Material::REFLECTIVE);
	entity->GetMaterial ()->Ambient () = Color (0.7, 0.7, 1.0);
	entity->GetMaterial ()->Diffuse () = Color (0.1, 0.1, 0.1);
	entity->GetMaterial ()->Specular () = Color (0.9, 0.9, 0.9);
	entity->GetMaterial ()->Shininess () = 40.0;
	entity->GetMaterial ()->Reflectivity () =  1.0;
	entity->GetMaterial ()->DiffuseReflectivity () = 0.0;
	entity->GetMaterial ()->Refractiveness () = 0.0;
	entity->GetMaterial ()->RefractionIndex () = 1.5;
	entity->GetMaterial ()->Density () = 0.15;
	scene->Add (entity);
	
    // Other Sphere
    entity = new Sphere (Vector(3.5, 1.8, 4.3), 1.3);
    entity->GetMaterial ()->SetProperties (Material::TEXTURE | Material::AMBIENT | Material::DIFFUSE | Material::SPECULAR);
    Procedural *lava = new Lava ();
    lava->AddColor (-1.0, Color::RED);
    lava->AddColor (-0.125, Color::RED.BlendLerp (Color::YELLOW, 0.5));
    lava->AddColor (0.0, Color::YELLOW);
    lava->AddColor (0.125, Color::RED.BlendLerp (Color::YELLOW, 0.5));
    lava->AddColor (1.0, Color::RED);
    entity->GetMaterial ()->SetTexture (lava);
    entity->GetMaterial ()->Ambient () = Color (0.7, 0.7, 0.7);
    entity->GetMaterial ()->Diffuse () = Color (0.2, 0.2, 0.2);
    entity->GetMaterial ()->Specular () = Color (0.8, 0.8, 0.8);
    entity->GetMaterial ()->Shininess () = 20.0;
    entity->GetMaterial ()->Reflectivity () = 0.0;
    entity->GetMaterial ()->DiffuseReflectivity () = 0.0;
    entity->GetMaterial ()->Refractiveness () = 0.0;
    entity->GetMaterial ()->RefractionIndex () = 1.5;
    entity->GetMaterial ()->Density () = 0.15;
    scene->Add (entity);

	// First Light Source
	entity = new Light (Vector (0.0, 5.0, 5.0), Light::POINT);
	entity->GetMaterial ()->SetProperties (Material::EMISSION);
	entity->GetMaterial ()->Emission () = Color(0.4, 0.4, 0.4);
	scene->Add (entity);
	
	// Second Light Source
	entity = new Light (Vector(2.0, 5.0, 1.0), Light::POINT);
	entity->GetMaterial ()->SetProperties (Material::EMISSION);
	entity->GetMaterial ()->Emission () = Color(0.6, 0.6, 0.8);
	scene->Add (entity);
	
	// Third Light Source
	entity = new Light (Vector (-6.0, 4.0, 11.0), Light::POINT);
	entity->Orientation () = Vector (0.0, -1.0, 0.0);
	entity->GetMaterial ()->SetProperties (Material::EMISSION);
	entity->GetMaterial ()->Emission () = Color (0.8, 0.6, 0.6);
	scene->Add (entity);

	return scene;
}

/*---------------------------------------------------------------------------*/

Scene *CreateComplexScene () {
	Scene *scene = new Scene ();
	Entity *entity = NULL;

	entity = new Plane (Vector (0.0, 1.0, 0.0), 4.4);
	entity->GetMaterial ()->SetProperties (Material::AMBIENT | Material::DIFFUSE | Material::SPECULAR);
	entity->GetMaterial ()->SetTexture (new Checkerboard ());
	// entity->GetMaterial ()->SetTexture (GenerateCheckerboard ());
	entity->GetMaterial ()->Ambient () = Color (0.5, 0.3, 0.3);
	entity->GetMaterial ()->Diffuse () = Color::WHITE;
	entity->GetMaterial ()->Specular () = Color::BLACK;
	entity->GetMaterial ()->Reflectivity () = 0.0;
	entity->GetMaterial ()->DiffuseReflectivity () = 0.0;
	entity->GetMaterial ()->Refractiveness () = 0.0;
	entity->GetMaterial ()->RefractionIndex () = 1.5;
	entity->GetMaterial ()->Density () = 0.15;
	scene->Add (entity);

	entity = new Sphere (Vector (2.0, 0.8, 3.0), 2.5);
	entity->GetMaterial ()->SetProperties (Material::AMBIENT | Material::DIFFUSE | Material::SPECULAR | Material::REFLECTIVE | Material::REFRACTIVE);
	entity->GetMaterial ()->Ambient () = Color::LIGHT_GRAY;
	entity->GetMaterial ()->Diffuse () = Color (0.2, 0.2, 0.2);
	entity->GetMaterial ()->Specular () = Color (0.8, 0.8, 0.8);
	entity->GetMaterial ()->Reflectivity () = 0.2;
	entity->GetMaterial ()->Refractiveness () = 0.8;
	entity->GetMaterial ()->RefractionIndex () = 1.3;
	scene->Add (entity);

	entity = new Sphere (Vector (-5.5, -0.5, 7.0), 2.0);
	entity->GetMaterial ()->SetProperties (Material::AMBIENT | Material::DIFFUSE | Material::SPECULAR | Material::REFLECTIVE);
	entity->GetMaterial ()->Ambient () = Color (0.7, 0.7, 1.0);
	entity->GetMaterial ()->Diffuse () = Color (0.1, 0.1, 0.1);
	entity->GetMaterial ()->Specular () = Color (0.9, 0.9, 0.9);
	entity->GetMaterial ()->Reflectivity () = 0.5;
	entity->GetMaterial ()->Refractiveness () = 0.0;
	entity->GetMaterial ()->RefractionIndex () = 1.5;
	scene->Add (entity);

	entity = new Light (Vector (0.0, 5.0, 5.0), Light::POINT);
	entity->GetMaterial ()->SetProperties (Material::EMISSION);
	entity->GetMaterial ()->Emission () = Color (0.4, 0.4, 0.4);
	scene->Add (entity);

	entity = new Light (Vector (-3.0, 5.0, 1.0), Light::POINT);
	entity->GetMaterial ()->SetProperties (Material::EMISSION);
	entity->GetMaterial ()->Emission () = Color (0.6, 0.6, 0.6);
	scene->Add (entity);
	
	entity = new Light (Vector (-6.0, 4.0, 11.0), Light::POINT);
	entity->GetMaterial ()->SetProperties (Material::EMISSION);
	entity->GetMaterial ()->Emission () = Color (0.7, 1.0, 0.8);
	scene->Add (entity);

	entity = new Light (Vector (0.0, 4.0, 100.0), Light::POINT);
	entity->GetMaterial ()->SetProperties (Material::EMISSION);
	entity->GetMaterial ()->Emission () = Color::LIGHT_GRAY;
	scene->Add (entity);
	
	entity = new Sphere (Vector (-1.5, -3.8, 1.0), 1.5);
	entity->GetMaterial ()->SetProperties (Material::AMBIENT | Material::DIFFUSE | Material::SPECULAR | Material::REFRACTIVE);
	entity->GetMaterial ()->Ambient() = Color (1.0, 0.4, 0.4);
	entity->GetMaterial ()->Diffuse () = Color (0.2, 0.2, 0.2);
	entity->GetMaterial ()->Specular () = Color (0.8, 0.8, 0.8);
	entity->GetMaterial ()->Reflectivity () = 0.0;
	entity->GetMaterial ()->Refractiveness () = 0.8;
	entity->GetMaterial ()->RefractionIndex () = 1.5;
	entity->GetMaterial ()->Density () = 0.25;
	scene->Add (entity);

	entity = new Plane (Vector (0.4, 0.0, -1.0), 12.0);
	entity->GetMaterial ()->SetProperties (Material::AMBIENT | Material::DIFFUSE);
	entity->GetMaterial ()->Ambient () = Color (0.5, 0.3, 0.5);
	entity->GetMaterial ()->Diffuse () = Color (0.6, 0.6, 0.6);
	entity->GetMaterial ()->Specular () = Color::BLACK;
	entity->GetMaterial ()->Reflectivity () = 0.0;
	entity->GetMaterial ()->Refractiveness () = 0.0;
	entity->GetMaterial ()->RefractionIndex () = 1.5;
	// scene->Add (entity);

	entity = new Plane (Vector (0.0, -1.0, 0.0), 7.4);
	entity->GetMaterial ()->SetProperties (Material::AMBIENT | Material::DIFFUSE | Material::REFLECTIVE);
	entity->GetMaterial ()->Ambient () = Color (0.9, 0.9, 0.9);
	entity->GetMaterial ()->Diffuse () = Color::GRAY;
	entity->GetMaterial ()->Specular () = Color::BLACK;
	entity->GetMaterial ()->Reflectivity () = 0.0; // 0.1;
	entity->GetMaterial ()->DiffuseReflectivity () = 0.0;// 0.3;
	entity->GetMaterial ()->Refractiveness () = 0.0;
	entity->GetMaterial ()->RefractionIndex () = 1.5;
	scene->Add (entity);

	for (int x = 0; x < 8; ++x) {
		for (int y = 0; y < 7; ++y) {
			entity = new Sphere (Vector (-4.5 + x * 1.5, -3.3 + y * 1.5, 10.0), 0.3);
			entity->GetMaterial ()->SetProperties (Material::AMBIENT | Material::DIFFUSE | Material::SPECULAR | Material::REFLECTIVE);
			entity->GetMaterial ()->Ambient () =  Color (0.3, 1.0, 0.4);
			entity->GetMaterial ()->Diffuse () = Color (0.6, 0.6, 0.6);
			entity->GetMaterial ()->Specular () = Color (0.6, 0.6, 0.6);
			entity->GetMaterial ()->Reflectivity () = 0.2;
			entity->GetMaterial ()->DiffuseReflectivity () = 0.8;
			entity->GetMaterial ()->Refractiveness () = 0.0;
			entity->GetMaterial ()->RefractionIndex () = 1.5;
			scene->Add (entity);
		}
	}

	return scene;
}

/*---------------------------------------------------------------------------*/

#ifndef SIMPLE_SCENE
Scene *(*CreateScene) () = CreateComplexScene;
#else
Scene *(*CreateScene) () = CreateSimpleScene;
#endif

/*---------------------------------------------------------------------------*/

int main () {
	GC_INIT ();

	niesoft::Logger::setLevel (niesoft::Logger::TRACE);

#if 0
	Procedural *texture = new BasicPerlin ();
	texture->AddColor (-1.0, Color::WHITE);
	texture->AddColor (0.5, Color::BLACK);
	texture->AddColor (1.0, Color::WHITE);
	WriteCanvasToTgaImage ("texture.tga", (*texture));

	WriteCanvasToTgaImage ("check.tga", (*(new Checkerboard ())));

#else
	unsigned int *buffer = new (GC) unsigned int[SCREEN_WIDTH * SCREEN_HEIGHT];

	niesoft::Logger::debug ("Creating scene");

	Camera *camera = new Camera (SCREEN_WIDTH, SCREEN_HEIGHT, Vector (0.0, 0.0, -5.0), -4.0, 3.0, 4.0, -3.0);
	Scene *scene = CreateScene ();

	RayTracer *tracer = new RayTracer ();

	tracer->Initialize (camera, scene);
	
	niesoft::Logger::debug ("Beginning rendering");

#ifdef APPLE_PPC
	struct timeval start = { 0 };
	gettimeofday (&start, NULL);
#endif
	
	tracer->Render (buffer);
	
#ifdef APPLE_PPC
	struct timeval now = { 0 };
	gettimeofday (&now, NULL);
	
	unsigned int ticks = (now.tv_sec - start.tv_sec) * 1000 + (now.tv_usec - start.tv_usec) / 1000;
	double secs = (double) ticks / 1000.0;
	int mins = (int) secs / 60;
	secs -= (double) mins * 60.0;
	
	printf ("Completed rendering in %d:%2.3f.\n", mins, secs);
#endif

	WriteTgaImage ("sample.tga", buffer, SCREEN_WIDTH, SCREEN_HEIGHT);
#endif

	CHECK_LEAKS ();
}
