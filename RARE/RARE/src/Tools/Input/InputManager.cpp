#include "InputManager.h"

namespace Rare {


	//std::unique_ptr<InputManager> InputManager::_instance;
	std::vector<Rare::KeyCode>  InputManager::_keyPressedEvents;
	std::vector<Rare::KeyCode> InputManager::_keyReleasedEvents;

	glm::vec2 InputManager::_mousePosition;
	std::vector<glm::vec2>  InputManager::_mouseMovedEvents;
	std::vector<Rare::MouseKey>  InputManager::_mousePressEvents;
	std::vector<Rare::MouseKey>  InputManager::_mouseReleaseEvents;

}