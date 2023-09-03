#pragma once

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/vector2.hpp>

using namespace godot;

struct Projectile {
	Vector2 position;
	Vector2 offset;
	double rotation;

	Ref<Resource> shape_ref;
	uint32_t collision_mask;

	Callable callback;
	TypedArray<Callable> trajectory_callbacks;
};

class ProjectileServer2D : public Node2D {
	GDCLASS(ProjectileServer2D, Node2D);

	Vector<Projectile> projectiles;

	void _update(double p_delta);

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	void spawn_projectile(const Projectile &p_projectile);

	PackedFloat32Array get_buffer() const;

	ProjectileServer2D();
};
