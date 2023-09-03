#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/vector2.hpp>

using namespace godot;

class ProjectileTrajectory2D : public Node {
	GDCLASS(ProjectileTrajectory2D, Node);

	Callable callback;

protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	Vector2 get_trajectory(const Vector2 &p_velocity) const;

	void set_trajectory_callback(const Callable &p_callback);

	PackedStringArray _get_configuration_warnings() const override;
};
