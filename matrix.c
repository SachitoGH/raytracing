#include "raytracing.h"

matrix matrix_identity(int size)
{
    matrix m = { .size = size };
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            m.data[i][j] = (i == j) ? 1.0f : 0.0f;
    return m;
}

matrix matrix_zero(int size)
{
    matrix m = { .size = size };
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            m.data[i][j] = 0.0f;
    return m;
}

int matrix_equal(matrix a, matrix b)
{
    if (a.size != b.size) return 0;
    for (int i = 0; i < a.size; ++i)
        for (int j = 0; j < a.size; ++j)
            if (fabs(a.data[i][j] - b.data[i][j]) > EPSILON)
                return 0;
    return 1;
}


matrix matrix_multiply(matrix a, matrix b)
{
    matrix result = matrix_zero(a.size);
    for (int i = 0; i < a.size; ++i)
        for (int j = 0; j < a.size; ++j)
            for (int k = 0; k < a.size; ++k)
                result.data[i][j] += a.data[i][k] * b.data[k][j];
    return result;
}

tuple matrix_multiply_tuple(matrix m, tuple t)
{
    float x = m.data[0][0]*t.x + m.data[0][1]*t.y + m.data[0][2]*t.z + m.data[0][3]*t.w;
    float y = m.data[1][0]*t.x + m.data[1][1]*t.y + m.data[1][2]*t.z + m.data[1][3]*t.w;
    float z = m.data[2][0]*t.x + m.data[2][1]*t.y + m.data[2][2]*t.z + m.data[2][3]*t.w;
    float w = m.data[3][0]*t.x + m.data[3][1]*t.y + m.data[3][2]*t.z + m.data[3][3]*t.w;
    return (tuple){ x, y, z, w };
}

matrix matrix_transpose(matrix m)
{
    matrix result = { .size = m.size };
    for (int i = 0; i < m.size; ++i)
	{
        for (int j = 0; j < m.size; ++j)
		{
            result.data[i][j] = m.data[j][i];
        }
    }
    return (result);
}


void print_matrix(matrix m)
{
    for (int i = 0; i < m.size; ++i) {
        for (int j = 0; j < m.size; ++j) {
            printf("%6.2f ", m.data[i][j]);
        }
        printf("\n");
    }
}

float determinant_2x2(matrix m)
{
    return ((m.data[0][0] * m.data[1][1]) - (m.data[0][1] * m.data[1][0]));
}

matrix submatrix(matrix m, int row, int col)
{
	matrix result = { .size = m.size - 1 };
	int res_row = 0;

	for (int i = 0; i < m.size; ++i)
	{
		if (i == row) continue;
		int res_col = 0;

		for (int j = 0; j < m.size; ++j)
		{
			if (j == col) continue;
			result.data[res_row][res_col] = m.data[i][j];
			res_col++;
		}
		res_row++;
	}
	return (result);
}

float cofactor(matrix m, int row, int col)
{
    float minor_val = minor(m, row, col);
    return (((row + col) % 2 == 0) ? minor_val : -minor_val);
}

int is_invertible(matrix m)
{
    return (fabs(determinant(m)) > EPSILON);
}


float minor(matrix m, int row, int col)
{
    matrix sub = submatrix(m, row, col);
    return (determinant(sub));
}


float determinant(matrix m)
{
    if (m.size == 2)
    {
        return (determinant_2x2(m));
    }

    float det = 0.0f;
    for (int col = 0; col < m.size; ++col)
    {
        det += m.data[0][col] * cofactor(m, 0, col);
    }

    return (det);
}

matrix inverse(matrix m)
{
    float det = determinant(m);
    matrix result = { .size = m.size };

    if (!is_invertible(m))
    {
        printf("Matrix is not invertible!\n");
        return (result);
    }

    for (int row = 0; row < m.size; ++row)
    {
        for (int col = 0; col < m.size; ++col)
        {
            float c = cofactor(m, row, col);
            result.data[col][row] = c / det; // fix: no transpose here
        }
    }

    return (result);
}


// transformation

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
    float rad = angle * M_PI / 180.0f; // Convert angle to radians
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
    float rad = angle * M_PI / 180.0f; // Convert angle to radians
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
    float rad = angle * M_PI / 180.0f; // Convert angle to radians
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
