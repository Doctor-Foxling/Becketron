#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/detail/type_quat.hpp>
#include <PhysX/PxPhysicsAPI.h>
#include "PhysXManager.h""

#define USE_PHYSX

#ifdef USE_PHYSX

namespace Becketron {

	class PhysXRigidbody
	{
	public:
		PhysXRigidbody(glm::vec3 pos, glm::quat rot, glm::vec3 scale, physx::PxMaterial* material);
		~PhysXRigidbody();

		glm::mat4 GetTransform();

		glm::vec3 GetPos();

		glm::quat GetRot();

		void SetMass(float mass);
		void SetDensity(float density);
		void SetLinearVelocity(physx::PxVec3 l_Velocity);
		void SetAngularVelocity(physx::PxVec3 a_Velocity);
		void addForce();
		void EnableGravity(bool en_Gravity);

		void BodyReset();

		void SetKinematic(bool kinematic);

		bool IsKinematic();

		physx::PxRigidDynamic* GetRigidbody() { return m_Body; }

	private:
		physx::PxRigidDynamic* m_Body;
		bool m_Kinematic = false;

		physx::PxTransform InitialTrans;
	};

}

#endif