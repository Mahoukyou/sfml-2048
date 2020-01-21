#include "scenemanager.h"

#include "scene.h"

// No need for any mutexes, since we are using a single thread anyway
SceneManager& SceneManager::instance()
{
	static SceneManager scene_manager{};
	return scene_manager;
}

void SceneManager::set_new_scene(std::unique_ptr<IScene> new_scene)
{
	pending_new_scene_ = std::move(new_scene);
}

void SceneManager::process_pending_scene()
{
	if (pending_new_scene_)
	{
		current_scene_ = std::move(pending_new_scene_.value());
		pending_new_scene_ = std::nullopt;
	}
}

IScene* SceneManager::current_scene() noexcept
{
	return current_scene_.get();
}

const IScene* SceneManager::current_scene() const noexcept
{
	return current_scene_.get();
}

