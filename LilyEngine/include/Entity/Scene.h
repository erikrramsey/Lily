#pragma once

#include "Entity/EC_registry.h"
#include "Entity/Components.h"

#include "Renderer/Renderer.h"

#include <vector>
#include <iterator>

namespace Lily {

class UObject;
class Scene {
    public:
		Scene();
		~Scene();

		void Init();
		void Update(long long dt);

		Camera& getCamera() { return m_camera; }

		UObject* create_UObject();
		void delete_UObject(UObject* obj);
		
		std::vector<UObject*> m_objects;
    private:
        //create scene camera class
        Camera m_camera;

		EC_registry m_registry;

		friend class UObject;
};

}
