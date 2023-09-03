#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/vector2.hpp>

using namespace godot;

class ProjectileSpawner2D : public Node {
	GDCLASS(ProjectileSpawner2D, Node);

	NodePath projectile_server_node;

	ObjectID cache;

	Ref<Resource> shape_ref;
	uint32_t collision_mask = 1;

	TypedArray<Callable> trajectory_callbacks;

	void _update_cache();

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void spawn_projectile(const Vector2 &p_position, double p_rotation, const Callable &p_callback) const;

	void set_projectile_server_node(const NodePath &p_projectile_server_node);
	NodePath get_projectile_server_node() const;

	void set_projectile_shape(const Ref<Resource> &p_shape_ref);
	Ref<Resource> get_projectile_shape() const;

	void set_projectile_collision_mask(uint32_t p_mask);
	uint32_t get_projectile_collision_mask() const;

	void set_projectile_collision_mask_value(int p_layer_number, bool p_value);
	bool get_projectile_collision_mask_value(int p_layer_number) const;

	void add_trajectory_callback(const Callable &p_callback);
	void remove_trajectory_callback(const Callable &p_callback);
	void clear_trajectory_callbacks();
};
