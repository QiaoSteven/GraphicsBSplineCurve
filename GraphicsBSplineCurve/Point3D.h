#pragma once
class Point3D
{
public:
	double x;
	double y;
	double z;

	Point3D();
	Point3D(double s);
	Point3D(double x, double y, double z);
	~Point3D();
	Point3D operator =(Point3D p) { x = p.x; y = p.y; z = p.z; return(*this); };
	Point3D operator +=(Point3D p) { x += p.x; y += p.y; z += p.z; return(*this); };
	Point3D operator -=(Point3D p) { x -= p.x; y -= p.y; z -= p.z; return(*this); };
	Point3D operator *=(double s) { x *= s; y *= s; z *= s; return(*this); };
	Point3D operator /=(double s) { x /= s; y /= s; z /= s; return(*this); };
	Point3D operator +(Point3D p) 
	{
		Point3D t;
		t.x = x + p.x;
		t.y = y + p.y;
		t.z = z + p.z;
		return(t);
	};

	Point3D operator -(Point3D p) 
	{
		Point3D t;
		t.x = x - p.x;
		t.y = y - p.y;
		t.z = z - p.z;
		return(t);
	};

	Point3D operator *(double s) 
	{
		Point3D t;
		t.x = x * s;
		t.y = y * s;
		t.z = z * s;
		return(t);
	};

	Point3D operator /(double s) 
	{
		Point3D t;
		t.x = x / s;
		t.y = y / s;
		t.z = z / s;
		return(t);
	};

	friend Point3D operator *(double s, Point3D p) 
	{
		Point3D t;
		t.x = p.x * s;
		t.y = p.y * s;
		t.z = p.z * s;
		return(t);
	};
};

