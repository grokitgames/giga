
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

// Engine specific assertion that allows a message to be provided
#define GIGA_ASSERT(test, message) { if((test) == false) printf(message); assert(test); }

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

#include <errno.h>
#include <fcntl.h>

// STL
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <list>

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

// SQLite
#define  SQLITE_OMIT_LOAD_EXTENSION
#include <sqlite3.h>
#include <sqlite3ext.h>

// MySQL
#include <my_global.h>
#include <mysql.h>

// libcurl (HTTP)
#include <curl/curl.h>

// JSON for JSON parsing
// #include <json.hpp>

// Engine includes
#include <Core/GigaObject.hpp>
#include <Core/Variant.hpp>
#include <Core/ObjectPool.hpp>
#include <Scripting/ScriptableVariant.hpp>
#include <Scripting/ScriptableObjectType.hpp>
#include <Scripting/ScriptableObject.hpp>
#include <IO/StorableObjectField.hpp>
#include <IO/StorableObjectType.hpp>
#include <IO/StorableObject.hpp>
#include <Core/Component.hpp>
#include <Core/ComponentTemplateType.hpp>
#include <Core/ComponentTemplate.hpp>
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
#include <IO/MemoryWriter.hpp>
#include <IO/DataLoader.hpp>
#include <IO/SQLiteDataLoader.hpp>
#include <IO/MySQLDataLoader.hpp>
#include <IO/Directory.hpp>
#include <Platform/DateTime.hpp>
#include <Platform/Window.hpp>
#include <Platform/Timer.hpp>
#include <IO/Profiler.hpp>
#include <Resource/Resource.hpp>
#include <Resource/ResourceObject.hpp>
#include <Resource/ResourceSystem.hpp>
#include <Scripting/Script.hpp>
#include <Scripting/ScriptComponent.hpp>
#include <Scripting/ScriptingSystem.hpp>
#include <Render/Color.hpp>
#include <Render/VertexBuffer.hpp>
#include <Render/IndexBuffer.hpp>
#include <Render/VertexAttributes.hpp>
#include <Render/VertexFormat.hpp>
#include <Render/BoundingBox.hpp>
#include <Render/Texture.hpp>
#include <Render/Texture2D.hpp>
#include <Render/Texture3D.hpp>
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
#include <Render/RenderComponent.hpp>
#include <Render/Vector3.hpp>
#include <Render/Quaternion.hpp>
#include <Render/StaticMeshComponent.hpp>
#include <Render/Plane.hpp>
#include <Render/Frustum.hpp>
#include <Render/CameraComponent.hpp>
#include <Render/Framebuffer.hpp>
#include <Render/RenderPass.hpp>
#include <Render/ShadowPass.hpp>
#include <Render/LightComponent.hpp>
#include <Render/Scene.hpp>
#include <Render/SpotLightComponent.hpp>
#include <Render/PostProcessPass.hpp>
#include <Render/RenderSystem.hpp>
#include <Network/UDPSocket.hpp>
#include <Network/Socket.hpp>
#include <Network/NetworkMessage.hpp>
#include <Network/Messages/EchoRequestMessage.hpp>
#include <Network/Messages/EchoResponseMessage.hpp>
#include <Network/NetworkSession.hpp>
#include <Network/NetworkSystem.hpp>
#include <Network/EntitySnapshot.hpp>
#include <Network/ReplicationSystem.hpp>
#include <Network/Messages/EntitySnapshotMessage.hpp>
#include <Network/Messages/CommandMessage.hpp>
#include <Network/Messages/ResendRequestMessage.hpp>
#include <Network/Messages/AckMessage.hpp>
#include <Network/HttpRequest.hpp>
#include <Core/Application.hpp>

#ifdef GIGA_USING_OPENGL
#include <Render/OpenGL/OpenGLVertexBuffer.hpp>
#include <Render/OpenGL/OpenGLIndexBuffer.hpp>
#include <Render/OpenGL/OpenGLVertexAttributes.hpp>
#include <Render/OpenGL/OpenGLTexture.hpp>
#include <Render/OpenGL/OpenGLTexture2D.hpp>
#include <Render/OpenGL/OpenGLTexture3D.hpp>
#include <Render/OpenGL/OpenGLShaderVariation.hpp>
#include <Render/OpenGL/OpenGLFramebuffer.hpp>
#include <Render/OpenGL/OpenGLGBufferPass.hpp>
#include <Render/OpenGL/OpenGLDeferredRenderPass.hpp>
#include <Render/OpenGL/OpenGLDeferredLightingPass.hpp>
#include <Render/OpenGL/OpenGLRenderSystem.hpp>
#include <Render/OpenGL/OpenGLShadowPass.hpp>
#endif

#endif
