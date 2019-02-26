#ifndef SPRINGDAMPER_H
#define SPRINGDAMPER_H

#include <memory>
class Particle;

class SpringDamper {
public:
	void ApplySpringForces();

public:
	std::shared_ptr<Particle> mP0;
	std::shared_ptr<Particle> mP1;
	
	float mSpringConstant;
	float mDampingFactor;
	float mRestLength;
	
};
#endif // !SPRINGDAMPER_H

