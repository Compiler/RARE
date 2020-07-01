#pragma once
#include "RareCommon.h"
namespace Rare {
	namespace ECS {
		
		namespace Handles {
			typedef unsigned int handle;
			constexpr handle TransformComponent = 0;
			constexpr handle RenderableComponent= 1;
		}

		struct Component {
			Handles::handle _HANDLE_;
		};
		struct TransformComponent : public Component{
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;
			TransformComponent() { _HANDLE_ = Handles::TransformComponent; }

		};
	


	}
}