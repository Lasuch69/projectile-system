extends ProjectileSpawner2D

func _ready() -> void:
	spawn_projectile(Vector2.ZERO, 0.0, projectile_callback)

#func _process(delta: float) -> void:
#	spawn()

func projectile_callback(p_result: Dictionary, p_position: Vector2) -> void:
	print(name, ", ", p_position)

func spawn() -> void:
	var projectile_position = random_point_in_circle(16.0)
	var velocity = Vector2.ZERO.direction_to(projectile_position) * 100.0;
	
	spawn_projectile(projectile_position, velocity, projectile_callback)

func random_point_in_circle(p_radius: float, p_on_surface: bool = false) -> Vector2:
	var radians: float = randf() * TAU
	
	if !p_on_surface:
		p_radius *= sqrt(randf())
	
	return Vector2(p_radius * cos(radians), p_radius * sin(radians))
