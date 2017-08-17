#pragma once

#define NOMINMAX
#include <algorithm>
#include <memory>
#include <set>

#include <lug/Core/Version.hpp>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Module.hpp>
#include <lug/Graphics/Render/Mesh.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Scene/Scene.hpp>

namespace lug {
namespace Graphics {

/**
 * @brief      Class for graphics.
 *
 *             This class is the root class of the 3D engine. It handles modules, will create
 *             the Graphics::Renderer based on the param rendererType given in the Graphics::InitInfo
 *             structure.
 *             It also handles Scene, Camera and Mesh creation.
 */
class LUG_GRAPHICS_API Graphics {
public:
    /**
     * @brief      Utility structure used to initialize the Graphics component.
     */
    struct InitInfo {
        Renderer::Type rendererType;            ///> The renderer to use
        Renderer::InitInfo rendererInitInfo;    ///> Initialization info for the renderer
        std::set<Module::Type> mandatoryModules;
        std::set<Module::Type> optionalModules;
    };

public:
    /**
     * @brief      Constructs a Graphics instance.
     *
     * @param[in]  appName     The application name.
     * @param[in]  appVersion  The application version.
     */
    Graphics(const std::string& appName, const Core::Version& appVersion);

    Graphics(const Graphics&) = delete;
    Graphics(Graphics&&) = delete;

    Graphics& operator=(const Graphics&) = delete;
    Graphics& operator=(Graphics&&) = delete;

    ~Graphics() = default;

    /**
     * @brief      Initializes the application with the informations filled in @p initInfo structure.
     *
     * @param[in]  initInfo  The initialize information.
     *
     * @return     @p true if the initialization was successful.
     */
    bool init(const InitInfo& initInfo);

    /**
     * @brief      Begin the initialization of the application with the informations filled in @p initInfo structure.
     *             Just create the lug::Graphics::Renderer and begin its initialization.
     *
     * @param[in]  initInfo  The initialize information.
     *
     * @return     @p true if the initialization was successful.
     */
    bool beginInit(const InitInfo& initInfo);

    /**
     * @brief      Finish the initialization of the application with the informations filled in @p initInfo structure.
     *             Finish the initialization of the lug::Graphics::Renderer.
     *
     * @return     @p true if the initialization was successful.
     */
    bool finishInit();

    /**
     * @brief      Check if a module is loaded
     *
     * @param[in]  type  The module type to check.
     *
     * @return     @p true if the module is loaded.
     */
    bool isModuleLoaded(Module::Type type) const;

    const std::set<Module::Type>& getLoadedMandatoryModules() const;
    const std::set<Module::Type>& getLoadedOptionalModules() const;

    void unsupportedModule(Module::Type type);

    /**
     * @brief      Gets the renderer.
     *
     * @return     A raw pointer to the renderer instance.
     */
    Renderer* getRenderer() const;

private:
    std::string _appName;
    Core::Version _appVersion;

    InitInfo _initInfo{
        /* rendererType     */ Renderer::Type::Vulkan,
        /* rendererInitInfo */ {},
        /* mandatoryModules */ {},
        /* optionalModules  */ {}
    };

    std::set<Module::Type> _loadedMandatoryModules{};
    std::set<Module::Type> _loadedOptionalModules{};

    std::unique_ptr<Renderer> _renderer{nullptr};
};

#include <lug/Graphics/Graphics.inl>

} // Graphics
} // lug
