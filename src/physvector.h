#ifndef _physvector_h
#define _physvector_h

typedef float PhysReal;

class PhysVector3
{
	public:
		PhysReal X;
		PhysReal Y;
		PhysReal Z;

		PhysVector3(PhysReal x, PhysReal y, PhysReal z);
};

#endif
