#include "btpch.h"
#include "PhysXRigidbody.h"
#include "PhysXManager.h"

namespace Becketron {

	PhysXRigidbody::PhysXRigidbody(glm::vec3 pos, glm::quat rot, glm::vec3 scale, physx::PxMaterial* material)
		: m_Material(material)
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
		
		// For restting purposes
		InitialTrans = Transform;
		// --

		m_Body = PhysXManager::s_PXPhysicsFactory->createRigidDynamic(Transform);
		
		physx::PxReal halfExtentX = scale.x / 2;
		physx::PxReal halfExtentY = scale.y / 2;
		physx::PxReal halfExtentZ = scale.z / 2;

		m_Shape = PhysXManager::s_PXPhysicsFactory->createShape(physx::PxBoxGeometry(
			halfExtentX, halfExtentY, halfExtentZ), *material);
		
		m_Body->attachShape(*m_Shape);
		//physx::PxMaterial* material2 = PhysXManager::s_PXPhysicsFactory->createMaterial(0.1f, 0.4f, 1.0f);
		//m_Shape->setMaterials(&material2, 1);
		m_Body->attachShape(*m_Shape);
		m_Body->setMass(1.0f);
		m_Body->userData = this;
		//m_Body->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);
	}

	PhysXRigidbody::~PhysXRigidbody()
	{
		//m_Shape->release();
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

	void PhysXRigidbody::BodyReset()
	{
		m_Body->setGlobalPose(InitialTrans);
	}

	void PhysXRigidbody::SetMass(float mass)
	{
		m_Body->setMass(mass);
	}

	void PhysXRigidbody::SetDensity(float density)
	{
		physx::PxRigidBodyExt::updateMassAndInertia(*m_Body, density);
	}

	void PhysXRigidbody::SetLinearVelocity(physx::PxVec3 l_Velocity)
	{
		m_Body->setLinearVelocity(l_Velocity);
	}

	void PhysXRigidbody::SetAngularVelocity(physx::PxVec3 a_Velocity)
	{
		m_Body->setAngularVelocity(a_Velocity);
	}

	void PhysXRigidbody::SetLinearDamping(physx::PxReal linDamp)
	{
		m_Body->setLinearDamping(linDamp);
	}

	void PhysXRigidbody::SetAngularDamping(physx::PxReal angDamp)
	{
		m_Body->setAngularDamping(angDamp);
	}

	void PhysXRigidbody::SetScale(glm::vec3 scale)
	{
		physx::PxReal halfExtentX = scale.x / 2;
		physx::PxReal halfExtentY = scale.y / 2;
		physx::PxReal halfExtentZ = scale.z / 2;

		m_Body->detachShape(*m_Shape);
		m_Shape->release();

		m_Shape = PhysXManager::s_PXPhysicsFactory->createShape(physx::PxBoxGeometry(
			halfExtentX, halfExtentY, halfExtentZ), *m_Material);

		m_Body->attachShape(*m_Shape);
	}

	void PhysXRigidbody::SetMaterial(physx::PxMaterial* mat) // a pointer to const pointer to a PxMaterial
	{
		m_Body->detachShape(*m_Shape);

		physx::PxMaterial* materials[] = { mat };
		m_Shape->setMaterials(materials, 1);

		m_Body->attachShape(*m_Shape);

		//m_Body->attachShape(*m_Shape);
	}

	void PhysXRigidbody::SetMaterial(float staticFric, float dynamicFric, float restitution)
	{
		physx::PxMaterial* material = PhysXManager::s_PXPhysicsFactory->createMaterial(staticFric,
			dynamicFric, restitution);
	}

	void PhysXRigidbody::AddForce(physx::PxVec3& force)
	{
		m_Body->addForce(force);
	}

	void PhysXRigidbody::AddTorque(physx::PxVec3& torque)
	{
		m_Body->addTorque(torque);
	}

	void PhysXRigidbody::EnableGravity(bool en_Gravity)
	{
		m_Body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, en_Gravity);
	}

	void PhysXRigidbody::SetKinematic(bool kinematic)
	{
		m_Body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, kinematic);
		m_Kinematic = kinematic;
		
		//m_Body->setLinearVelocity(physx::PxVec3(20.0f));
		/*
		if (kinematic)
		{
			const glm::vec3 position = GetPos();
			const glm::quat rotation = GetRot();

			m_Body->setKinematicTarget(physx::PxTransform(position.x, position.y, position.z, physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w)));
		}
		*/
	}


	bool PhysXRigidbody::IsKinematic()
	{
		return m_Body->getRigidBodyFlags().isSet(physx::PxRigidBodyFlag::eKINEMATIC);
	}
}