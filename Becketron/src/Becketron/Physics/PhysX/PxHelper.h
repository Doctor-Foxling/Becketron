#pragma once

#include <PhysX/PxPhysicsAPI.h>
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Becketron
{
	class PxHelper
	{
	public:
		static physx::PxTransform glmToPhysxTransform(const glm::mat4& Transform);

		static std::tuple<glm::vec3, glm::quat, glm::vec3>GetTransformDecomposition(
			const glm::mat4& transform);

		static physx::PxVec3 Vec3_glmToPx(const glm::vec3& value, float ts = 1.0f);
	};
}