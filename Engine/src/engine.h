#pragma once

#include "core/application/launcher.h"
#include "core/application/application.h"
#include "core/app_settings.h"

#include "core/debug/logger.h"
#include "core/debug/assert.h"

#include "core/input/input.h"

#include "core/resource/resource.h"
#include "core/resource/resource_manager.h"

#include "math/math_utils.h"
#include "math/vector/vector2.h"
#include "math/vector/vector3.h"
#include "math/vector/vector4.h"
#include "math/matrix/mat4.h"
#include "math/quaternion/quat.h"

#include "graphics/renderer/renderer.h"
#include "graphics/color/color.h"
#include "graphics/font/font.h"
#include "graphics/texture/texture.h"
#include "graphics/mesh/mesh.h"
#include "graphics/camera/camera.h"

#include "entity/entity.h"
#include "entity/entity_manager/entity_manager.h"


#include "components/2d/sprite.h"
#include "components/2d/transform_2d.h"
#include "components/3d/transform_3d.h"
#include "components/3d/mesh_instance.h"
#include "components/timer.h"
#include "components/particle/particle.h"
#include "components/test_component.h"

#include "systems/movement/movement_system.h"
#include "systems/particle/particle_system.h"
#include "systems/render/render_system.h"