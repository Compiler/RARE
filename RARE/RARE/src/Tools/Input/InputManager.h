#pragma once
#include <Tools/Input/RareInput.h>
#include <glm/vec2.hpp>
#include <vector>
#include <memory>
namespace Rare {

	class InputManager {

	private:
		//static std::unique_ptr<InputManager> _instance;
		static std::vector<Rare::KeyCode> _keyReleasedEvents;
		static std::vector<Rare::KeyCode>  _keyPressedEvents;
		static std::vector<Rare::MouseKey>  _mousePressEvents;
		static std::vector<Rare::MouseKey>_mouseReleaseEvents;
		static std::vector<glm::vec2>  _mouseMovedEvents;
		static glm::vec2 _mousePosition;
		/*InputManager() {};
		static void create() {
			static InputManager instance;
			_instance = std::make_unique<InputManager>(instance);
		}*/
	public:
		InputManager(const InputManager&) = delete;
		InputManager operator=(const InputManager&) = delete;

		/*static InputManager& getInstance() {
			if (!_instance) create();
			return *_instance;
		}*/
		static void addKeyPress(Rare::KeyCode key) {
			_keyPressedEvents.push_back(key);
		}
		static void addKeyRelease(Rare::KeyCode key) {
			_keyReleasedEvents.push_back(key);
		}
		static void addMousePosition(double x, double y) {
			_mousePosition.x = x;
			_mousePosition.y = y;
			_mouseMovedEvents.push_back(glm::vec2(x, y));
		}
		static void addMousePress(Rare::MouseKey keyPressed) {
			_mousePressEvents.push_back(keyPressed);
		}
		static void addMouseRelease(Rare::MouseKey keyReleased) {
			_mouseReleaseEvents.push_back(keyReleased);
		}
		static void clear() {
			_keyPressedEvents.clear();
			_keyReleasedEvents.clear();
			_mouseMovedEvents.clear();
			_mousePressEvents.clear();
			_mouseReleaseEvents.clear();
		}

		inline static bool isKeyPressed(Rare::KeyCode key) {
			for (int i = 0; i < _keyPressedEvents.size(); i++) {
				if (_keyPressedEvents[i] == key)
					return true;
			}
			return false;
		}
		inline static bool isKeyReleased(Rare::KeyCode key) {
			for (int i = 0; i < _keyReleasedEvents.size(); i++)
				if (_keyReleasedEvents[i] == key)
					return true;

			return false;
		}

		inline static bool isMousePressed(Rare::MouseKey key) {
			for (int i = 0; i < _mousePressEvents.size(); i++) {
				if (_mousePressEvents[i] == key)
					return true;
			}
			return false;
		}
		inline static bool isMouseReleased(Rare::MouseKey key) {
			for (int i = 0; i < _mouseReleaseEvents.size(); i++) {
				if (_mouseReleaseEvents[i] == key)
					return true;
			}
			return false;
		}

		inline static glm::vec2 getMouseMovedPosition() {
			return _mousePosition;
		}

	};


}