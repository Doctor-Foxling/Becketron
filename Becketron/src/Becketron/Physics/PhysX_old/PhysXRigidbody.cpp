#include "btpch.h"
#include "PhysXRigidbody.h"

#ifdef USE_OLD_PHYSX

namespace Becketron {

	PhysXRigidbody::PhysXRigidbody(Ref<PhysXScene> scene, glm::vec3 pos, glm::quat rot) : m_Scene(scene)
	{
		physx::PxVec3 PxPos;
		PxPos.x = pos.x;
		PxPos.y = pos.y;
		PxPos.z = pos.z;


		physx::PxQuat PxQua;
		PxQua.x = rot.x;
		PxQua.y = rot.y;
		PxQua.z = rot.z;
		PxQua.w = rot.w;

		physx::PxTransform Transform(PxPos, PxQua);

		m_Body = m_Scene->m_Physics->createRigidDynamic(Transform);
		m_Body->userData = this;
		m_Body->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);
		m_Scene->m_PhysXScene->addActor(*m_Body);
	}

	PhysXRigidbody::~PhysXRigidbody()
	{
		m_Scene->m_Physics->release();
	}

	glm::vec3 PhysXRigidbody::GetPos()
	{
		float xpos = m_Body->getGlobalPose().p.x;
		float ypos = m_Body->getGlobalPose().p.y;
		float zpos = m_Body->getGlobalPose().p.z;

		glm::vec3 pos;

		pos.x = xpos;
		pos.y = ypos;
		pos.z = zpos;

		return  pos;
	}

	glm::quat PhysXRigidbody::GetRot()
	{
		auto xq = m_Body->getGlobalPose().q.x;
		auto yq = m_Body->getGlobalPose().q.y;
		auto zq = m_Body->getGlobalPose().q.z;
		auto wq = m_Body->getGlobalPose().q.w;

		glm::quat q;
		q.x = xq;
		q.y = yq;
		q.z = zq;
		q.w = wq;

		return q;
	}

	void PhysXRigidbody::SetKinematic(bool kinematic)
	{
		if (kinematic)
		{
			m_Body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
			m_Kinematic = true;
		}
		else if (!kinematic)
		{
			m_Body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);
			m_Kinematic = false;
		}

	}

	void PhysXRigidbody::SetKinematic(bool kinematic)
	{
		m_Body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, kinematic);
		m_Kinematic = kinematic;

		if (kinematic)
		{
			const glm::vec3 position = GetPos();
			const glm::quat rotation = GetRot();

			m_Body->setKinematicTarget(physx::PxTransform(position.x, position.y, position.z, physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w)));
		}
	}


	bool PhysXRigidbody::IsKinematic()
	{
		return m_Body->getRigidBodyFlags().isSet(physx::PxRigidBodyFlag::eKINEMATIC);
	}
	void PhysXRigidbody::SetMass(float mass)
	{
	}
	void PhysXRigidbody::SetDensity(float density)
	{
	}
	void PhysXRigidbody::SetLinearVelocity(physx::PxVec3 l_Velocity)
	{
	}
	void PhysXRigidbody::SetAngularVelocity(physx::PxVec3 a_Velocity)
	{
	}
	void PhysXRigidbody::addForce()
	{
	}
	void PhysXRigidbody::EnableGravity(bool en_Gravity)
	{
	}
}

#endif