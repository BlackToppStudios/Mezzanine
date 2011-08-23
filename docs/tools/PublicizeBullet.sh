#!/bin/bash
# This exposes some private members in bullet so that they are readily available to the Physgame engine
#
# This needs to be run by the person updating bullet code prior to commiting a fresh bullet version.


echo "Updating btPoint2PointConstraint.h"
sed -i -e 's|	int			m_flags;|	public: //Made public by BTS with PublicizeBullet.sh\n	int			m_flags;\n	private:|' ../../physgameengine/libincludes/common/bulletsrc/bulletsvnsrc/src/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h

echo "Updating btHingeConstraint.h"
sed -i -e 's|	btScalar	m_motorTargetVelocity;|	public: //Made public by BTS with PublicizeBullet.sh\n	btScalar	m_motorTargetVelocity;|' ../../physgameengine/libincludes/common/bulletsrc/bulletsvnsrc/src/BulletDynamics/ConstraintSolver/btHingeConstraint.h

echo "Updating btConeTwistConstraint.h"
sed -i -e 's|	int			m_flags;|	public: //Made public by BTS with PublicizeBullet.sh\n	int			m_flags;\n	private:|' ../../physgameengine/libincludes/common/bulletsrc/bulletsvnsrc/src/BulletDynamics/ConstraintSolver/btConeTwistConstraint.h

# I know this one was protected and could have been done some other way, but I would rather keep it consistent with the others
echo "Updating btGeneric6DofConstraint.h"
sed -i -e 's|int			m_flags;|public: //Made public by BTS with PublicizeBullet.sh\n	int			m_flags;\n	private:|' ../../physgameengine/libincludes/common/bulletsrc/bulletsvnsrc/src/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h

echo "Updating btSliderConstraint.h"
sed -i -e 's|	int m_flags;|	public: //Made public by BTS with PublicizeBullet.sh\n	int			m_flags;\n	private:|' ../../physgameengine/libincludes/common/bulletsrc/bulletsvnsrc/src/BulletDynamics/ConstraintSolver/btSliderConstraint.h



