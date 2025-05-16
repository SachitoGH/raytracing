#include "raytracing.h"

material create_glass_material(void)
{
    material m;
    m.color = color(1.0f, 1.0f, 1.0f);
    m.pattern.type = UNSET;
    m.ambient = 0.0f;
    m.diffuse = 0.0f;
    m.specular = 0.9f;
    m.shininess = 300.0f;
    m.reflective = 0.9f;
    m.transparency = 1.0f;
    m.refractive_index = 1.5f;
    return m;
}

material create_mirror_material(void)
{
    material m;
    m.color = color(1.0f, 1.0f, 1.0f); // White, as a neutral fallback
    m.pattern.type = UNSET;            // No pattern
    m.ambient = 0.0f;                  // No ambient contribution
    m.diffuse = 0.0f;                  // No diffuse scattering
    m.specular = 1.0f;                 // Strong specular highlights
    m.shininess = 300.0f;              // Sharp highlights
    m.reflective = 1.0f;               // Perfect reflection
    m.transparency = 0.0f;             // Opaque
    m.refractive_index = 1.0f;         // Ignored, default to air
    return m;
}

material create_matte_material(tuple color)
{
    material m;
    m.color = color;
    m.pattern.type = UNSET;
    m.ambient = 0.1f;
    m.diffuse = 0.9f;
    m.specular = 0.0f;
    m.shininess = 200.0f;
    m.reflective = 0.0f;
    m.transparency = 0.0f;
    m.refractive_index = 1.0f;
    return m;
}

material create_metal_material(tuple color)
{
    material m;
    m.color = color;
    m.pattern.type = UNSET;
    m.ambient = 0.1f;
    m.diffuse = 0.7f;
    m.specular = 0.3f;
    m.shininess = 100.0f;
    m.reflective = 0.5f;
    m.transparency = 0.0f;
    m.refractive_index = 1.0f;
    return m;
}

material	create_material(void)
{
	material	m;
	m.color = color(1, 1, 1);
	m.ambient = 0.1f;
	m.diffuse = 0.9f;
	m.specular = 0.9f;
	m.shininess = 200.0f;
	m.pattern.type = UNSET;
	m.reflective = 0.0f;
	m.transparency = 0.0f;
	m.refractive_index = 1.0f;
	return (m);
}

