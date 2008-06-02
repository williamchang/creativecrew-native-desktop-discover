/**
@file
    Utilities.h
@brief
    Coming soon.
@author
    William Chang
@version
    0.1
@date
    - Created: 2008-06-01
    - Modified: 2008-06-01
    .
@note
    References:
    - Ogre:
        - http://www.ogre3d.org/wiki/index.php/Raycasting_to_the_polygon_level
        .
    .
*/

#ifndef __Utilities_H__
#define __Utilities_H__

#include "EnginePrerequisites.h"

namespace Discover {

/** @class Utilities */
class Utilities {
public:
    /// Default constructor.
    Utilities();
    /// Default destructor.
    virtual ~Utilities();

    /// Instantiate this.
    bool instantiateThis();

    /// Cast ray from point.
    bool castRayFromPoint(const Ogre::Vector3& point, const Ogre::Vector3& direction, Ogre::Vector3& result);
    /// Get mesh information.
    static void getMeshInformation(const Ogre::MeshPtr mesh, size_t& vertex_count, Ogre::Vector3*& vertices, size_t& index_count, unsigned long*& indices, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale);
protected:
    Ogre::RaySceneQuery* m_pray_scene_query;
};

} // END namespace Discover

#endif // #ifndef __Utilities_H__