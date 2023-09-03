class_name LinearTrajectory2D
extends ProjectileTrajectory2D

var delta_time: float = 0.0

func _ready() -> void:
	get_parent().add_trajectory_callback(_get_projectile_velocity)

func _physics_process(delta: float) -> void:
	delta_time += delta

func _get_projectile_velocity(p_parameters: Dictionary) -> Dictionary:
	p_parameters["offset"] = Vector2(sin(delta_time * 2) * 50.0, 0.0);
	return p_parameters
