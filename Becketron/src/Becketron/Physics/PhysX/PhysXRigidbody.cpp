#include "btpch.h"
#include "PhysXRigidbody.h"
#include "PhysXManager.h"

namespace Becketron {

	PhysXRigidbody::PhysXRigidbody(glm::vec3 pos, glm::quat rot, glm::vec3 scale, physx::PxMaterial* material)
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

		m_Body = PhysXManager::s_PXPhysicsFactory->createRigidDynamic(Transform);
		
		physx::PxReal halfExtentX = scale.x / 2;
		physx::PxReal halfExtentY = scale.y / 2;
		physx::PxReal halfExtentZ = scale.z / 2;

		physx::PxShape* shape = PhysXManager::s_PXPhysicsFactory->createShape(physx::PxBoxGeometry(
			halfExtentX, halfExtentY, halfExtentZ), *material);

		m_Body->attachShape(*shape);
		m_Body->userData = this;
		shape->release();
		//m_Body->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);
	}

	PhysXRigidbody::~PhysXRigidbody()
	{
	
		//m_Scene->m_Physics->release();
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

	glm::mat4 PhysXRigidbody::GetTransform()
	{
		auto xpos = m_Body->getGlobalPose().p.x;
		auto ypos = m_Body->getGlobalPose().p.y;
		auto zpos = m_Body->getGlobalPose().p.z;

		auto xq = m_Body->getGlobalPose().q.x;
		auto yq = m_Body->getGlobalPose().q.y;
		auto zq = m_Body->getGlobalPose().q.z;

		auto pos = glm::mat4(xq * ypos * zpos);
		auto rot = glm::mat4(xpos * yq * zq);

		return glm::mat4(pos * rot);

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
}