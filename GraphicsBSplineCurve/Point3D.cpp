#include "stdafx.h"
#include "Point3D.h"

Point3D::Point3D() : x(0), y(0), z(0)
{
}


Point3D::~Point3D()
{
}



Point3D::Point3D(double s) : x(s), y(s), z(s)
{
}

Point3D::Point3D(double x, double y, double z) : x(x), y(y), z(z)
{
}
