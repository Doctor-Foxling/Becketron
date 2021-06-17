#include "btpch.h"
#include "PxHelper.h"

#include <glm/gtx/matrix_decompose.hpp>

namespace Becketron
{

	physx::PxTransform PxHelper::glmToPhysxTransform(const glm::mat4& Transform)
	{
		physx::PxVec3 PxPos;
		const glm::mat4& transform = Transform;
		auto [pos, rot, sacle] = GetTransformDecomposition(transform);
		PxPos.x = pos.x;
		PxPos.y = pos.y;
		PxPos.z = pos.z;

		physx::PxQuat PxQua;
		PxQua.x = rot.x;
		PxQua.y = rot.y;
		PxQua.z = rot.z;
		PxQua.w = rot.w;

		physx::PxTransform PhysXTransform(PxPos, PxQua);
		return PhysXTransform;
	}

	std::tuple<glm::vec3, glm::quat, glm::vec3> PxHelper::GetTransformDecomposition(
		const glm::mat4& transform)
	{
		glm::vec3 scale, translation, skew;
		glm::vec4 perspective;
		glm::quat orientation;
		glm::decompose(transform, scale, orientation, translation, skew, perspective);

		return { translation, orientation, scale };
	}

	physx::PxVec3 PxHelper::Vec3_glmToPx(const glm::vec3& value, float ts)
	{
		// ts here is optional, in case we are passing in timestep as well
		float x = value.x * ts;
		float y = value.y * ts;
		float z = value.z * ts;
		physx::PxVec3 vel = physx::PxVec3(x, y, z);

		return vel;
	}

}