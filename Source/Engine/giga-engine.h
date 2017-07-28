
#ifndef giga_engine_h
#define giga_engine_h

// Disable compiler warnings about using STL and dynamic linking
#pragma warning(disable:4251)

// Windows DLL export definition
#ifndef GIGA_API
    #ifdef GIGA_EXPORTS
        // We are building this library
        #define GIGA_API __declspec(dllexport)
    #else
        // We are using this library
        #define GIGA_API __declspec(dllimport)
    #endif
#endif

// Base math includes; _USE_MATH_DEFINES gives us constants like PI
#define _USE_MATH_DEFINES
#include <cmath>

// Windows definitions - NOMINMAX defaults to STL; WIN32_LEAN_AND_MEAN minimizes Windows' includes
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN         1

// Base system includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>

// STL
#include <vector>
#include <map>
#include <string>
#include <algorithm>

// GLM (math)
#define GLM_FORCE_RADIANS       1
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Typedefs for easy access
typedef glm::vec2 vector2;
typedef glm::vec3 vector3;
typedef glm::vec4 color;
typedef glm::vec4 vector4;
typedef glm::quat quaternion;
typedef glm::mat4 matrix4;

// V8 for scripting
#include <libplatform/libplatform.h>
#include <v8.h>

// Engine includes
#include <Core/GigaObject.hpp>
#include <Core/Variant.hpp>
#include <Core/ObjectPool.hpp>
#include <Scripting/ScriptableVariant.hpp>
#include <Scripting/ScriptableObjectType.hpp>
#include <Scripting/ScriptableObject.hpp>
#include <Core/Component.hpp>
#include <Core/Entity.hpp>
#include <Core/System.hpp>
#include <Core/EntitySystem.hpp>
#include <Core/EventData.hpp>
#include <Core/Event.hpp>
#include <Core/EventSystem.hpp>
#include <Core/Error.hpp>
#include <Core/ErrorSystem.hpp>
#include <IO/InputDevice.hpp>
#include <IO/Command.hpp>
#include <IO/InputSystem.hpp>
#include <IO/Keyboard.hpp>
#include <IO/File.hpp>
#include <IO/MemoryReader.hpp>
#include <Platform/DateTime.hpp>
#include <Platform/Window.hpp>
#include <Platform/Timer.hpp>
#include <Resource/Resource.hpp>
#include <Resource/ResourceObject.hpp>
#include <Resource/ResourceSystem.hpp>
#include <Scripting/Script.hpp>
#include <Scripting/ScriptComponent.hpp>
#include <Scripting/ScriptingSystem.hpp>
#include <Render/VertexBuffer.hpp>
#include <Render/IndexBuffer.hpp>
#include <Render/VertexAttribObject.hpp>
#include <Render/VertexFormat.hpp>
#include <Render/BoundingBox.hpp>
#include <Render/Texture2D.hpp>
#include <Render/Shader.hpp>
#include <Render/ShaderVariation.hpp>
#include <Render/ShaderSystem.hpp>
#include <Render/Material.hpp>
#include <Render/MaterialSystem.hpp>
#include <Render/Bone.hpp>
#include <Render/AnimationTransform.hpp>
#include <Render/AnimationFrame.hpp>
#include <Render/Animation.hpp>
#include <Render/Mesh.hpp>
#include <Render/AnimationInstance.hpp>
#include <Render/Transform.hpp>
#include <Render/TransformComponent.hpp>
#include <Render/SceneComponent.hpp>
#include <Render/Vector3.hpp>
#include <Render/Quaternion.hpp>
#include <Render/StaticMeshComponent.hpp>
#include <Render/RenderSystem.hpp>
#include <Core/Application.hpp>

#endif
