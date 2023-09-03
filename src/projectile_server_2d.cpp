#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/physics_direct_space_state2d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters2d.hpp>
#include <godot_cpp/classes/world2d.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/variant.hpp>

#include "projectile_server_2d.hpp"

using namespace godot;

void ProjectileServer2D::spawn_projectile(const Projectile &p_projectile) {
	projectiles.append(p_projectile);
}

PackedFloat32Array ProjectileServer2D::get_buffer() const {
	int buffer_size = Math::max(projectiles.size() * 8, 8);

	PackedFloat32Array buffer;
	buffer.resize(buffer_size);

	for (int i = 0; i < projectiles.size(); i++) {
		int instance_idx = i * 8;

		Vector2 position = projectiles[i].position + projectiles[i].offset;
		float rotation = projectiles[i].rotation;

		// basis x
		buffer[instance_idx + 0] = Math::cos(rotation);
		buffer[instance_idx + 1] = Math::sin(rotation);

		// basis y
		buffer[instance_idx + 4] = -Math::sin(rotation);
		buffer[instance_idx + 5] = Math::cos(rotation);

		// origin
		buffer[instance_idx + 3] = position.x;
		buffer[instance_idx + 7] = position.y;
	}

	return buffer;
}

void ProjectileServer2D::_update(const double p_delta) {
	Vector<int> to_remove;

	for (int i = 0; i < projectiles.size(); i++) {
		Transform2D transform = Transform2D(projectiles[i].rotation, projectiles[i].position + projectiles[i].offset);

		PhysicsShapeQueryParameters2D parameters = {};
		parameters.set_transform(transform);
		parameters.set_shape(projectiles[i].shape_ref);
		parameters.set_collision_mask(projectiles[i].collision_mask);

		TypedArray<Dictionary> result = get_world_2d()->get_direct_space_state()->intersect_shape(&parameters, 1);

		if (!result.is_empty()) {
			Array args;
			args.resize(2);
			args[0] = result[0];
			args[1] = projectiles[i].position;

			projectiles[i].callback.callv(args);

			to_remove.append(i);
			continue;
		}

		TypedArray<Callable> trajectory_callbacks = projectiles[i].trajectory_callbacks;

		Dictionary values;
		values["position"] = projectiles[i].position;
		values["offset"] = projectiles[i].offset;
		values["rotation"] = projectiles[i].rotation;

		for (int i = 0; i < trajectory_callbacks.size(); i++) {
			Array args;
			args.append(values);

			Callable callback = trajectory_callbacks[i];

			values = callback.callv(args);
		}

		projectiles.write[i].position = values["position"];
		projectiles.write[i].offset = values["offset"];
		projectiles.write[i].rotation = values["rotation"];
	}

	for (int i = to_remove.size(); i > 0; i--) {
		projectiles.remove_at(to_remove[i - 1]);
	}
}

void ProjectileServer2D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_INTERNAL_PHYSICS_PROCESS:
			if (Engine::get_singleton()->is_editor_hint()) {
				break;
			}

			double delta = get_physics_process_delta_time();
			_update(delta);
			break;
	}
}

void ProjectileServer2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_buffer"), &ProjectileServer2D::get_buffer);
}

ProjectileServer2D::ProjectileServer2D() {
	// Initialize any variables here.

	projectiles = Vector<Projectile>();
	set_physics_process_internal(true);
}
