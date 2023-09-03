#include "projectile_spawner_2d.hpp"
#include "projectile_server_2d.hpp"

using namespace godot;

void ProjectileSpawner2D::spawn_projectile(const Vector2 &p_position, double p_rotation, const Callable &p_callable) const {
	ERR_FAIL_COND(shape_ref.is_null());

	if (!is_inside_tree()) {
		return;
	}

	if (cache.is_null()) {
		return;
	}

	ProjectileServer2D *n = Object::cast_to<ProjectileServer2D>(ObjectDB::get_instance(cache));

	if (!n) {
		return;
	}

	if (!n->is_inside_tree()) {
		return;
	}

	n->spawn_projectile({ p_position, Vector2(0.0, 0.0), p_rotation, shape_ref, collision_mask, p_callable, trajectory_callbacks });
}

void ProjectileSpawner2D::set_projectile_server_node(const NodePath &p_projectile_server_node) {
	projectile_server_node = p_projectile_server_node;

	if (is_inside_tree()) {
		_update_cache();
	}
}

NodePath ProjectileSpawner2D::get_projectile_server_node() const {
	return projectile_server_node;
}

void ProjectileSpawner2D::set_projectile_shape(const Ref<Resource> &p_shape_ref) {
	ERR_FAIL_COND(p_shape_ref.is_null());
	shape_ref = p_shape_ref;
}

Ref<Resource> ProjectileSpawner2D::get_projectile_shape() const {
	return shape_ref;
}

void ProjectileSpawner2D::set_projectile_collision_mask(uint32_t p_mask) {
	collision_mask = p_mask;
}

uint32_t ProjectileSpawner2D::get_projectile_collision_mask() const {
	return collision_mask;
}

void ProjectileSpawner2D::set_projectile_collision_mask_value(int p_layer_number, bool p_value) {
	ERR_FAIL_COND_MSG(p_layer_number < 1, "Collision layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_MSG(p_layer_number > 32, "Collision layer number must be between 1 and 32 inclusive.");
	uint32_t mask = get_projectile_collision_mask();
	if (p_value) {
		mask |= 1 << (p_layer_number - 1);
	} else {
		mask &= ~(1 << (p_layer_number - 1));
	}
	set_projectile_collision_mask(mask);
}

bool ProjectileSpawner2D::get_projectile_collision_mask_value(int p_layer_number) const {
	ERR_FAIL_COND_V_MSG(p_layer_number < 1, false, "Collision layer number must be between 1 and 32 inclusive.");
	ERR_FAIL_COND_V_MSG(p_layer_number > 32, false, "Collision layer number must be between 1 and 32 inclusive.");
	return get_projectile_collision_mask() & (1 << (p_layer_number - 1));
}

void ProjectileSpawner2D::add_trajectory_callback(const Callable &p_callback) {
	trajectory_callbacks.append(p_callback);
}

void ProjectileSpawner2D::remove_trajectory_callback(const Callable &p_callback) {
	int idx = trajectory_callbacks.find(p_callback);

	if (idx != -1) {
		trajectory_callbacks.remove_at(idx);
	}
}

void ProjectileSpawner2D::clear_trajectory_callbacks() {
	trajectory_callbacks.clear();
}

void ProjectileSpawner2D::_update_cache() {
	cache = ObjectID();

	if (has_node(projectile_server_node)) {
		Node *node = get_node<Node>(projectile_server_node);

		cache = node->get_instance_id();
	}
}

void ProjectileSpawner2D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			_update_cache();
		} break;
	}
}

void ProjectileSpawner2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("spawn_projectile", "position", "rotation", "callback"), &ProjectileSpawner2D::spawn_projectile);

	ClassDB::bind_method(D_METHOD("set_projectile_server_node", "projectile_server_node"), &ProjectileSpawner2D::set_projectile_server_node);
	ClassDB::bind_method(D_METHOD("get_projectile_server_node"), &ProjectileSpawner2D::get_projectile_server_node);

	ClassDB::bind_method(D_METHOD("set_projectile_shape", "shape"), &ProjectileSpawner2D::set_projectile_shape);
	ClassDB::bind_method(D_METHOD("get_projectile_shape"), &ProjectileSpawner2D::get_projectile_shape);

	ClassDB::bind_method(D_METHOD("set_projectile_collision_mask", "mask"), &ProjectileSpawner2D::set_projectile_collision_mask);
	ClassDB::bind_method(D_METHOD("get_projectile_collision_mask"), &ProjectileSpawner2D::get_projectile_collision_mask);

	ClassDB::bind_method(D_METHOD("set_projectile_collision_mask_value", "layer_number", "value"), &ProjectileSpawner2D::set_projectile_collision_mask_value);
	ClassDB::bind_method(D_METHOD("get_projectile_collision_mask_value", "layer_number"), &ProjectileSpawner2D::get_projectile_collision_mask_value);

	ClassDB::bind_method(D_METHOD("add_trajectory_callback", "callback"), &ProjectileSpawner2D::add_trajectory_callback);
	ClassDB::bind_method(D_METHOD("remove_trajectory_callback", "callback"), &ProjectileSpawner2D::remove_trajectory_callback);
	ClassDB::bind_method(D_METHOD("clear_trajectory_callbacks"), &ProjectileSpawner2D::clear_trajectory_callbacks);

	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "projectile_server_path", PROPERTY_HINT_NODE_PATH_VALID_TYPES, "ProjectileServer2D"), "set_projectile_server_node", "get_projectile_server_node");

	ADD_GROUP("Projectile", "projectile_");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "projectile_shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape2D"), "set_projectile_shape", "get_projectile_shape");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "projectile_collision_mask", PROPERTY_HINT_LAYERS_2D_PHYSICS), "set_projectile_collision_mask", "get_projectile_collision_mask");
}
