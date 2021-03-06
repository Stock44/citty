//
// Created by hiram on 6/6/22.
//

#ifndef CITYY_RENDERINGSYSTEM_H
#define CITYY_RENDERINGSYSTEM_H

#include <queue>
#include <optional>
#include <set>
#include "../engine/System.h"
#include "components/Mesh.h"
#include "../components/Transform.h"
#include "components/Camera.h"
#include "components/Color.h"
#include "../Window.h"
#include "Shader.h"
#include "buffers/VertexBuffer.h"
#include "buffers/IndexBuffer.h"
#include "VertexArray.h"
#include "buffers/ColorVertexBuffer.h"
#include "buffers/ModelMatBuffer.h"
#include "components/MeshRef.h"

namespace graphics {
    class GLADInitError : public std::runtime_error {
    public:
        GLADInitError() : std::runtime_error("Failed to initialize GLAD") {};
    };

    using engine::Entity;
    using engine::EntitySet;

    struct RenderCommand {
        long meshID;
        long bufferPosition;

        bool bucketWith(RenderCommand const &other) const {
            return meshID == other.meshID;
        }

        bool operator==(RenderCommand const &rhs) const = default;

        std::weak_ordering operator<=>(RenderCommand const &rhs) const {
            // Lexicographical comparison, first meshID then bufferPosition
            return std::tie(meshID, bufferPosition) <=> std::tie(rhs.meshID, rhs.bufferPosition);
        }
    };

    template<typename T>
    struct DerefLess {
        bool operator()(std::unique_ptr<T> const &lhs, std::unique_ptr<T> const &rhs) const {
            return *lhs < *rhs;
        }
    };

    struct MeshRecord {
        ModelMatBuffer matBuffer;
        ColorVertexBuffer colorBuffer;
        VertexArray arrayObject; // VAO of this specific mesh (should be bound to engine VBO and modelMats of this mesh
        long verticesIndex;
        long indicesIndex;
        long meshSize;
        bool dirty;
    };

    class RenderingSystem : public engine::System {
    public:
        explicit RenderingSystem(Window &newSize);

        void setup(engine::ComponentManager &componentManager) override;

        void update(engine::EntityManager &elementManager) override;

        void stageMeshIntoBuffers(Mesh const &mesh);

        void regenerateBuckets();

        void tryRegisterEntity(Entity entity);

        void onMeshCreate(EntitySet entities);

        void onTransformCreate(EntitySet entities);

        void onColorCreate(EntitySet entities);

        void onTransformUpdate(EntitySet entities);

        void onColorUpdate(EntitySet entities);

        void onCameraCreate(EntitySet entities);

        void setViewportSize(const std::pair<int, int> &newViewportSize);

        void updateViewMatrix(Transform const &transform);

        void updateProjectionMatrix(Camera camera);

    private:
        Window &window;
        std::unique_ptr<Shader> shader;


        std::optional<Entity> cameraEntity;

        std::pair<int, int> viewportSize;

        std::unique_ptr<VertexBuffer> vertexBuffer;
        std::unique_ptr<IndexBuffer> indexBuffer;

        std::vector<std::unique_ptr<RenderCommand>> renderCommands;
        std::unordered_map<Entity, RenderCommand*> entityRenderMap;
        std::unordered_map<long, MeshRecord> loadedMeshes;

        // Bucket ranges, start position and size
        std::vector<std::pair<int, int>> buckets;

        engine::ComponentStore<MeshRef> *meshStore = nullptr;
        engine::ComponentStore<Color> *colorStore = nullptr;
        engine::ComponentStore<Transform> *transformStore = nullptr;
        engine::ComponentStore<Camera> *cameraStore = nullptr;
    };

} // graphics

#endif //CITYY_RENDERINGSYSTEM_H
