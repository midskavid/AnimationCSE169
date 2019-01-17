#ifndef BALLJOINT_H
#define BALLJOINT_H

// Forward Declare..
#include "Joint.h"

class BallJoint : public Joint {
public:
	virtual void MakeLocalMatrix() override;
};
#endif // BALLJOINT_H

