#pragma once
#include <memory>
#include <optional>

class IScene;

class SceneManager final
{
public:
	~SceneManager() = default;

	SceneManager(const SceneManager&) = delete;
	SceneManager(SceneManager&&) noexcept = delete;

	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager& operator=(SceneManager&&) noexcept = delete;

	static SceneManager& instance();

	void set_new_scene(std::unique_ptr<IScene> new_scene);
	void process_pending_scene();
	
	[[nodiscard]] IScene* current_scene() noexcept;
	[[nodiscard]] const IScene* current_scene() const noexcept;

private:
	SceneManager() = default;

	std::optional<std::unique_ptr<IScene>> pending_new_scene_;
	std::unique_ptr<IScene> current_scene_;
};
