#pragma once

#include "Entity/ECRegistry.h"
#include "Entity/Component.h"
#include "Entity/Transform.h"
#include "Entity/Family.h"
#include "Entity/Importer.h"
#include "Entity/SceneSerializer.h"

#include "Renderer/Renderer.h"
#include "Renderer/Light.h"

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iterator>

namespace Lily {

class SceneSerializer;
class Lobject;
class Importer;
class Scene {
public:
	Scene();
	~Scene();

	void Init();
	void update(long long dt);
    void clear();

	Lobject* get(Entity ent);
    Lobject* get_root();
	Lobject& get_camera() { return *m_camera; }
	Lobject* create_Lobject();
	void delete_Lobject(Lobject* obj);
    void clear_Lobject(Lobject* obj);
	
	ECRegistry m_registry;
	std::unordered_map<Entity, Lobject*> m_objects;
private:
	Lobject* m_camera;
    Lobject* m_root;

	friend class Lobject;
};
}
