#include "raytracing.h"

void	set_transform(shape *s, matrix t)
{
	s->transform = t;
	s->inverse_transform = inverse(&t);
}

matrix translation(float tx, float ty, float tz)
{
    matrix result = matrix_identity(4);  // Start with the identity matrix (4x4)
    
    // Set the translation values
    result.data[0][3] = tx;
    result.data[1][3] = ty;
    result.data[2][3] = tz;

    return result;
}

matrix scaling(float sx, float sy, float sz)
{
    matrix result = matrix_identity(4);  // Start with the identity matrix (4x4)

    // Set the scaling values
    result.data[0][0] = sx;
    result.data[1][1] = sy;
    result.data[2][2] = sz;

    return result;
}

// Rotation around the X-axis
matrix rotation_x(float angle)
{
    matrix result = matrix_identity(4);
    float rad = angle * DEG_RADIANTS; // Convert angle to radians
    result.data[1][1] = cosf(rad);
    result.data[1][2] = -sinf(rad);
    result.data[2][1] = sinf(rad);
    result.data[2][2] = cosf(rad);
    return result;
}

// Rotation around the Y-axis
matrix rotation_y(float angle)
{
    matrix result = matrix_identity(4);
    float rad = angle * DEG_RADIANTS; // Convert angle to radians
    result.data[0][0] = cosf(rad);
    result.data[0][2] = sinf(rad);
    result.data[2][0] = -sinf(rad);
    result.data[2][2] = cosf(rad);
    return result;
}

// Rotation around the Z-axis
matrix rotation_z(float angle)
{
    matrix result = matrix_identity(4);
    float rad = angle * DEG_RADIANTS; // Convert angle to radians
    result.data[0][0] = cosf(rad);
    result.data[0][1] = -sinf(rad);
    result.data[1][0] = sinf(rad);
    result.data[1][1] = cosf(rad);
    return result;
}

matrix shearing(float xy, float xz, float yx, float yz, float zx, float zy)
{
    matrix result = matrix_identity(4);

    result.data[0][1] = xy;
    result.data[0][2] = xz;

    result.data[1][0] = yx;
    result.data[1][2] = yz;

    result.data[2][0] = zx;
    result.data[2][1] = zy;

    return (result);
}
