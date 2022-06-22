#include "Entity/SceneSerializer.h"

using json = nlohmann::json;
namespace Lily {

static void serialize_Lobject_rec(Scene* scene, Lobject* obj, json& parent);
static void deserialize_Lobject_rec(Scene* scene, Lobject* obj, json& parent);

SceneSerializer::SceneSerializer() = default;

void SceneSerializer::serialize(Scene* scene, const fs::path& path) {
    json jsonLobjectArray;
    std::ofstream outfile(path);

    if (scene == nullptr) {
        outfile << jsonLobjectArray.dump(4);
        outfile.close();
        return;
    }

    if (!outfile.good()) {
        std::cout << "Problem opening out file" << std::endl;
    }

    for (auto& obj : scene->get_root()->get_children()) {
        serialize_Lobject_rec(scene, obj, jsonLobjectArray);
    }

    outfile << jsonLobjectArray.dump(4);
    outfile.close();
}

void SceneSerializer::deserialize(Scene* scene, const fs::path& path) {
    scene->clear();
    scene->Init();
    std::ifstream infile(path);
    json fullJson;
    infile >> fullJson;
    for (auto& jsn : fullJson) {
        auto obj = scene->create_Lobject();
        scene->get_root()->add_child(obj);
        deserialize_Lobject_rec(scene, obj, jsn);
    }
    infile.close();
}

void serialize_Lobject_rec(Scene* scene, Lobject* obj, json& parent) {
    json cur;
    json children;
    for (auto child : obj->get_children()) {
        serialize_Lobject_rec(scene, child, children);
    }

    cur["Children"] = children;

    cur["Name"] = obj->get_name();
    auto tran = obj->get<Transform>();
    json transform;
    transform["Position"]["x"] = tran.get_pos().x;
    transform["Position"]["y"] = tran.get_pos().y;
    transform["Position"]["z"] = tran.get_pos().z;

    transform["Rotation"]["x"] = tran.get_rot().x;
    transform["Rotation"]["y"] = tran.get_rot().y;
    transform["Rotation"]["z"] = tran.get_rot().z;

    transform["Scale"]["x"] = tran.get_sca().x;
    transform["Scale"]["y"] = tran.get_sca().y;
    transform["Scale"]["z"] = tran.get_sca().z;

    cur["Transform"] = transform;

    auto fam = obj->get<Family>();


    auto mesh = obj->try_get<Mesh>();
    if (mesh) {
        cur["Mesh"]["Path"] = mesh->import_path;
        cur["Mesh"]["Material"] = mesh->material_path;
    }

    auto light = obj->try_get<Light>();
    if (light) {
        cur["Light"]["Exists"] = "True";
    }
    parent[obj->get_name()] = cur;
}


void deserialize_Lobject_rec(Scene* scene, Lobject* obj, json& parent) {
    for (json& child : parent["Children"]) {
        auto ch = scene->create_Lobject();
        obj->add_child(ch);
        deserialize_Lobject_rec(scene, ch, child);
    }
    obj->set_name(parent["Name"].get<std::string>().c_str());

    auto& tran = obj->get<Transform>();
    glm::vec3 pos = {
        parent["Transform"]["Position"]["x"],
        parent["Transform"]["Position"]["y"],
        parent["Transform"]["Position"]["z"]
    };

    glm::vec3 rot {
        parent["Transform"]["Rotation"]["x"],
        parent["Transform"]["Rotation"]["y"],
        parent["Transform"]["Rotation"]["z"]
    };

    glm::vec3 sca {
            parent["Transform"]["Scale"]["x"],
            parent["Transform"]["Scale"]["y"],
            parent["Transform"]["Scale"]["z"]
    };

    tran.set_pos(pos);
    tran.set_rot(rot);
    tran.set_sca(sca);

    if (parent.contains("Mesh")) {
        json mj = parent["Mesh"];
        auto m = Mesh(obj->m_entity, mj["Path"]);
        if (mj.contains("Material"))
            m.material_path = mj["Material"];
        Importer::load_imported_mesh(m);
        obj->add_component(m);
    }

    if (parent.contains("Light")) {
        Light l(obj->m_entity);
        obj->add_component(l);
    }
}

}
