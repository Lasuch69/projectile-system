extends MultiMeshInstance2D

const INSTANCE_COUNT: int = 10000

@export var projectile_server: ProjectileServer2D

func _ready() -> void:
	multimesh.instance_count = INSTANCE_COUNT

func _process(delta: float) -> void:
	var buffer: PackedFloat32Array = projectile_server.get_buffer()
	multimesh.visible_instance_count = int(buffer.size() / 8.0)
	
	buffer.resize(INSTANCE_COUNT * 8)
	multimesh.buffer = buffer
