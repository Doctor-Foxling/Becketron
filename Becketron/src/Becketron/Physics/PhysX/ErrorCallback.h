#pragma once

#include "Becketron/Core/Core.h"
#include "Becketron.h"

//fdef USE_NVIDIA

#include <PhysX/PxPhysicsAPI.h>
//#include <pxshared/foundation/PxErrorCallback.h>

namespace Becketron {

	class PhysXErrorCallback : public physx::PxErrorCallback
	{
	public:
		virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file,
			int line)
		{
			switch (code)
			{
			case physx::PxErrorCode::Enum::eDEBUG_INFO:
				BT_CORE_INFO("{0}", message);
				break;

			case physx::PxErrorCode::Enum::eDEBUG_WARNING:
				BT_CORE_WARN("{0}", message);
				break;

			case physx::PxErrorCode::Enum::eOUT_OF_MEMORY:
				BT_CORE_CRITICAL("Out of Memory! {0}", message);
				break;

			case physx::PxErrorCode::Enum::eINVALID_PARAMETER:
				BT_CORE_ERROR("Invalid parameter! {0}", message);
				break;

			case physx::PxErrorCode::Enum::eABORT:
				BT_CORE_ERROR("{0}!", message);
				break;

			case physx::PxErrorCode::Enum::ePERF_WARNING:
				BT_CORE_WARN("{0}!", message);
				break;

			case physx::PxErrorCode::Enum::eINTERNAL_ERROR:
				BT_CORE_ERROR("{0}!", message);
				break;

			case physx::PxErrorCode::Enum::eINVALID_OPERATION:
				BT_CORE_ERROR("{0}!", message);
				break;

			case physx::PxErrorCode::Enum::eMASK_ALL:
				BT_CORE_INFO("{0}!", message);
				break;

			case physx::PxErrorCode::Enum::eNO_ERROR:
				//BT_CORE_INFO("{0}!", message);
				break;

			default:
				break;
			}
		}
	};
}

//ndif