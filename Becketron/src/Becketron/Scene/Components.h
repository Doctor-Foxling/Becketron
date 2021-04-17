#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SceneCamera.h"
#include "Becketron/Physics/BT_Physics/PhysicsObject.h"
#include "Becketron/Renderer/Texture.h"

#include "ScriptableEntity.h"
#include "PhysicsEntity.h"
//#include "Becketron/Physics/PhysX/PhysXRigidbody.h"
#include <PhysX/PxRigidActor.h>
#include "Becketron/Physics/PhysX/PhysXRigidbody.h"

//#define BT_PHYSICS

//#define USE_OLD_PHYSX

#define USE_PHYSX

namespace Becketron {

	// -- define component types
	enum class ComponentType
	{
		Basic = 0,
		Cam,
		VisualA,
		Physics,
		Script
	};
	//

	struct TagComponent
	{
		ComponentType type = ComponentType::Basic;
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		ComponentType type = ComponentType::Basic;
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		// copy constructor
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			:Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

#ifdef USE_OLD_PHYSX

	struct PhysXRigidbodyComponent
	{
		ComponentType type = ComponentType::Physics;
		bool isKinematic;

		PhysXRigidbody* m_body;

		PhysXRigidbodyComponent() = default;
		PhysXRigidbodyComponent(PhysXRigidbody* m_Body, bool IsKinematic) 
			: m_body(m_Body), isKinematic(IsKinematic) {}
	};

#endif

#ifdef USE_PHYSX

	struct PhysXRigidDynamicComponent
	{
		ComponentType type = ComponentType::Physics;
		bool isKinematic;

		// Testing
		//physx::PxTransform m_Transform;
		physx::PxMaterial* material = NULL;
		float staticFriction = 0.5f;
		float dynamicFriction = 0.5f;
		float restitution = 0.0f;
		
		float mass = 1.0f;
		float density = 1.0f;
		float linearVelocity = 0.0f;
		float angularVelocity = 0.0f;
		float linearDamping = 0.0f;
		float angularDamping = 0.0f;

		glm::vec3 scale ; // will be used to check if the object scale has been changed
		//physx::PxReal m_HalfExtent = physx::PxReal(1.0f);
		physx::PxShape* shape;
		Ref<PhysXRigidbody> rigidbody;

		// old values
		float old_staticFriction = 0.5f;
		float old_dynamicFriction = 0.5f;
		float old_restitution = 0.5f;
		float old_mass = 1.0f;
		float old_density = 1.0f;
		float old_linearVelocity = 0.0f;
		float old_angularVelocity = 0.0f;
		float old_linearDamping = 0.0f;
		float old_angularDamping = 0.0f;

		//m_Material = PhysXManager::s_PXPhysicsFactory->createMaterial(0.5f, 0.5f, 0.6f);
		

			/*= CreateRef<PxRigidBody>(physx::PxTransform PxTF, physx::PxShape * PxShape,
			physx::PxMaterial * PxMaterial = m_Material);*/

		PhysXRigidDynamicComponent() = default;

		PhysXRigidDynamicComponent(physx::PxMaterial* mat, bool IsKinematic)
			: material(mat), isKinematic(IsKinematic) {}
		PhysXRigidDynamicComponent(float StaticFriction = 0.5f, float DynamicFriction = 0.5f, 
			float Restitution = 0.5f, float Mass = 1.0f, bool IsKinematic = false)
			: staticFriction(StaticFriction), dynamicFriction(DynamicFriction), 
			restitution(Restitution), mass(Mass), isKinematic(IsKinematic) {}
	};
#endif

	struct SpriteRendererComponent
	{
		ComponentType type = ComponentType::VisualA;
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CubeRendererComponent
	{
		ComponentType type = ComponentType::VisualA;
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		CubeRendererComponent() = default;
		CubeRendererComponent(const CubeRendererComponent&) = default;
		CubeRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct TexturedCubeComponent
	{
		ComponentType type = ComponentType::VisualA;
		Ref<Texture2D> texture = nullptr;
		float tiling_factor = 1.0f;
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		TexturedCubeComponent() = default;
		TexturedCubeComponent(const TexturedCubeComponent&) = default;
		TexturedCubeComponent(Ref<Texture2D>& tex, float tilingFactor, const glm::vec4& color)
			: texture(tex), tiling_factor(tilingFactor), Color(color) {}
	};

	struct LightCubeComponent
	{
		ComponentType type = ComponentType::VisualA;
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		LightCubeComponent() = default;
		LightCubeComponent(const LightCubeComponent&) = default;
		LightCubeComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		ComponentType type = ComponentType::Cam;
		bool Primary = true;  // TODO: think about moving to Scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

#ifdef BT_PHYSICS

	struct PhysicsComponent
	{
		ComponentType type = Physics;
		PhysicsObject* physicsObject;
		bool controllable = false;

		PhysicsComponent() = default;
		PhysicsComponent(const PhysicsComponent&) = default;
		PhysicsComponent(PhysicsObject* physObj)
			: physicsObject(physObj) {}
	};

#endif

	struct NativeScriptComponent
	{
		ComponentType type = ComponentType::Script;
		ScriptableEntity* Instance = nullptr;

		// Using function pointers instead of std::function
		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			// Since we are not capturing 'this' we simulate that by passeing a pointer to iteself as a paramenter
			DestroyScript = [](NativeScriptComponent* nsc) {delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	//struct PhysicsComponent
	//{
	//	PhysicsEntity* Instance = nullptr;
	//	using Instantiate_fn = std::function<PhysicsEntity*()>;
	//	Instantiate_fn InstantiatePhysics;
	//	//PhysicsEntity* (*InstantiatePhysics)();
	//	void (*DestroyPhysics)(PhysicsComponent*);

	//	template<typename T>
	//	void Bind(PhysicsObject& physObj)
	//	{
	//		InstantiatePhysics = [physObj]() { return static_cast<PhysicsEntity*>(new T(physObj)); };
	//		// simulating 'this' being captured
	//		DestroyPhysics = [](PhysicsComponent* phy) {delete phy->Instance; phy->Instance = nullptr; };
	//	}
	//};
}