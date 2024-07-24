/*#pragma once
#include "../Classes/HashedString.h"
#include "../../Utils/Math.h"
#include <glm/fwd.hpp>
#include "../Classes/Actor.h"
#include "CameraDirectComponent.h"
using namespace glm;
class CameraComponent {
public:
    HashedString mViewName;
   Vec3<float> mOrigin{};
   void CameraDirectLookSystemUtil(Actor&, CameraComponent const&, CameraDirectLookComponent&, Vec2<float> const&);
   Vec4<float> mFov{}; // mViewport? (Fov X, Fov Y, Near, Far)?
    int8_t padding[4]{ 0 };
};//void CameraDirectLookSystemUtil::_handleLookInput(EntityContext&, CameraComponent const&, CameraDirectLookComponent&, Vec2 const&)*/