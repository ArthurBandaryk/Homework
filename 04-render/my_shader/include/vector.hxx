#ifndef VECTOR_HXX
#define VECTOR_HXX
struct Vector
{
    float x;
    float y;
    float z;
    float w;
    Vector();
    Vector(float x, float y, float z, float w = 1.f);
};
#endif // VECTOR_HXX
