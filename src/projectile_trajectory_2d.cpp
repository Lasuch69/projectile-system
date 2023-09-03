#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/array.hpp>

#include "projectile_spawner_2d.hpp"
#include "projectile_trajectory_2d.hpp"

using namespace godot;

void ProjectileTrajectory2D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_MOVED_IN_PARENT: {
			if (Engine::get_singleton()->is_editor_hint()) {
				update_configuration_warnings();
			}
		} break;
	}
}

PackedStringArray ProjectileTrajectory2D::_get_configuration_warnings() const {
	PackedStringArray warnings = Node::_get_configuration_warnings();

	ProjectileSpawner2D *spawner = Object::cast_to<ProjectileSpawner2D>(get_parent());

	if (spawner == nullptr) {
		warnings.push_back("ProjectileTrajectory2D derived nodes should only provide trajectory callback. Please only use it as a child of ProjectileSpawner2D class.");
	}

	return warnings;
}

Vector2 ProjectileTrajectory2D::get_trajectory(const Vector2 &p_velocity) const {
	if (callback.is_null() || !callback.is_valid()) {
		return p_velocity;
	}

	Array args;
	args.resize(1);
	args[0] = p_velocity;

	return callback.callv(args);
}

void ProjectileTrajectory2D::set_trajectory_callback(const Callable &p_callback) {
	callback = p_callback;
}

void ProjectileTrajectory2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_trajectory", "velocity"), &ProjectileTrajectory2D::get_trajectory);
	ClassDB::bind_method(D_METHOD("set_trajectory_callback", "callback"), &ProjectileTrajectory2D::set_trajectory_callback);
}
