#!/bin/bash
# This exposes some private members in bullet so that they are readily available to the Mezzanine engine
#
# This needs to be run by the person updating bullet code prior to commiting a fresh bullet version.

# Make m_flags public
echo "Updating btPoint2PointConstraint.h"
sed -i -e 's|	int			m_flags;|	public: //Made public by BTS with PublicizeBullet.sh\n	int			m_flags;\n	private:|' ../../Mezzanine/libincludes/common/bulletsrc/bulletsvnsrc/src/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h

# Make m_motorTargetVelocity and everything until the next access specifier public
# Remark out the #define _BT_USE_CENTER_LIMIT_ 1
echo "Updating btHingeConstraint.h"
sed -i -e 's|	btScalar	m_motorTargetVelocity;|	public: //Made public by BTS with PublicizeBullet.sh\n	btScalar	m_motorTargetVelocity;|' ../../Mezzanine/libincludes/common/bulletsrc/bulletsvnsrc/src/BulletDynamics/ConstraintSolver/btHingeConstraint.h
sed -i -e 's|#define _BT_USE_CENTER_LIMIT_ 1|//#define _BT_USE_CENTER_LIMIT_ 1|' ../../Mezzanine/libincludes/common/bulletsrc/bulletsvnsrc/src/BulletDynamics/ConstraintSolver/btHingeConstraint.h

# Make m_flags public
echo "Updating btConeTwistConstraint.h"
sed -i -e 's|	int			m_flags;|	public: //Made public by BTS with PublicizeBullet.sh\n	int			m_flags;\n	private:|' ../../Mezzanine/libincludes/common/bulletsrc/bulletsvnsrc/src/BulletDynamics/ConstraintSolver/btConeTwistConstraint.h

# I know this one was protected and could have been done some other way, but I would rather keep it consistent with the others
# Make m_flags public
echo "Updating btGeneric6DofConstraint.h"
sed -i -e 's|int			m_flags;|public: //Made public by BTS with PublicizeBullet.sh\n	int			m_flags;\n	private:|' ../../Mezzanine/libincludes/common/bulletsrc/bulletsvnsrc/src/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h

# Make m_flags public
echo "Updating btSliderConstraint.h"
sed -i -e 's|	int m_flags;|	public: //Made public by BTS with PublicizeBullet.sh\n	int			m_flags;\n	private:|' ../../Mezzanine/libincludes/common/bulletsrc/bulletsvnsrc/src/BulletDynamics/ConstraintSolver/btSliderConstraint.h

#Make all the protected members public
sed -i -e 's|protected|public|' ../../Mezzanine/libincludes/common/bulletsrc/bulletsvnsrc/src/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h
