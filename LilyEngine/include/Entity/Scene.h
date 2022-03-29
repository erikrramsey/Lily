#pragma once

#include "Entity/ECRegistry.h"
#include "Entity/Lobject.h"
#include "Entity/Component.h"
#include "Entity/Transform.h"
#include "Entity/Family.h"
#include "Entity/Importer.h"

#include "Renderer/Renderer.h"

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iterator>

namespace Lily {

class Lobject;
class Importer;
class Scene {
public:
	Scene();
	~Scene();

	void Init();
	void update(long long dt);
	void import_component(Lobject* obj, std::string& path);

	Lobject* get(Entity ent);

	Lobject& getCamera() { return *m_camera; }

	Lobject* create_Lobject();
	void delete_Lobject(Lobject* obj);
	
	ECRegistry m_registry;
	std::unordered_map<Entity, Lobject*> m_objects;
private:
	Lobject* m_camera;
	Importer* m_importer;

	friend class Lobject;
};

}
