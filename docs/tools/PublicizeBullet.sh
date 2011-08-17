#!/bin/bash
# This exposes some private members in bullet so that they are readily available to the Physgame engine
#
# This needs to be run by the person updating bullet code prior to commiting a fresh bullet version.


echo "Updating btPoint2PointConstraint.cpp"
# ../../physgameengine/libincludes/common/bulletsrc/bulletsvnsrc/src/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp
#"	int			m_flags;"
sed -i -e 's|	int			m_flags;|	public: //Made public by BTS with PublicizeBullet.sh\n	int			m_flags;\n	private:|' ../../physgameengine/libincludes/common/bulletsrc/bulletsvnsrc/src/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h

