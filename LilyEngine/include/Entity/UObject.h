#pragma once

#include <vector>

#include "Components.h"
#include "Scene.h"

namespace Lily {

typedef unsigned long Entity;

void Test_registry(); 
// API to access an entity and it's components
// also keeps track of immediate family?
class UObject {
public: 
	UObject(Entity id, Scene* scene);
	UObject(Entity id, UObject& parent);

	~UObject();

	UObject get_root();
	const char* GetName() { return m_name.c_str(); }
	void SetName(char* name) { m_name = name; }

	UObject* add_child(UObject* child);

	
	template<class T>
	T& add_component();
	
	template<class T>
	T& add_component(T& comp);

	template<class T>
	T& get_component();

	template<class T>
	T* try_get_component();

	template<class T>
	void remove_component();

//private:
	UObject();

	std::string m_name;

	Entity m_id;
	Scene* m_scene;
	std::vector<UObject *> m_children; 
	UObject* m_parent = NULL;
};

template<class T>
T& UObject::add_component() {
	return m_scene->m_registry.emplace<T>(m_id);
}

template<class T>
T& UObject::add_component(T& comp) {
	return m_scene->m_registry.emplace(m_id, comp);
}

template<class T>
T& UObject::get_component() {
	return m_scene->m_registry.get<T>(m_id);
}

template<class T>
T* UObject::try_get_component() {
	return m_scene->m_registry.try_get<T>(m_id);
}

template<class T>
void UObject::remove_component() {
	m_scene->m_registry.remove_component<T>(m_id);
}

}

