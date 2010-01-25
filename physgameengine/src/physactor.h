#include "physvector.h"

class actorbase {

	private:
	PhysVector3 vector;
	PhysReal coefficient;
	PhysReal weight;
	PhysReal density;
	virtual bool move=true;

	public:
	actorbase () {};
	~actorbase () {};
	actorbase (PhysReal x, PhysReal y, PhysReal z, PhysReal c, PhysReal w, PhysReal d) {
		vector.x = x;
		vector.y = y;
		vector.z = z;
		coefficient = c;
		weight = w;
		density = d;
	}
	PhysReal GetLocation () {
		return (vector.x, vector.y, vector.z);
	}
	PhysReal GetCoefficient () {
		return (coefficient);
	}
	PhysReal GetWeight () {
		return (weight);
	}
	PhysReal GetDensity () {
		return (density);
	}
}

class actordyn: public actorbase {

	private:
	PhysVector3 speed;
	PhysVector3 accel;

	public:
	PhysReal GetSpeed () {
		return (speed.x, speed.y, speed.z);
	}
	PhysReal GetAccel () {
		return (accel.x, accel.y, accel.z);
	}
	void Halt () {
		accel.x = 0;
		accel.y = 0;
		accel.z = 0;
		speed.x = 0;
		speed.y = 0;
		speed.z = 0;
	}
}

class actorsta: public actorbase {

	private:
	bool move=false;
}
