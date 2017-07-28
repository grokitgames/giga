
#ifndef window_hpp
#define window_hpp

class GIGA_API Window : public ScriptableObject {
public:
    Window();
    ~Window() = default;
    
    GIGA_CLASS_NAME("Window")
    
    /**
     * Create a new window
     */
    void Create(std::string name, int width, int height, bool fullscreen);
    
    /**
     * Get pointer to internal window handle
     */
    void* GetWindowHandle() { return m_window; }
    
    /**
     * Get window properties
     */
    int GetWidth() { return m_width; }
    int GetHeight() { return m_height; }
    
    int GetFramebufferWidth() { return m_framebufferWidth; }
    int GetFramebufferHeight() { return m_framebufferHeight; }
    
    bool IsFullscreen() { return m_fullscreen; }
    std::string GetName() { return m_name; }
    
    /**
     * Whether the window is closed/closing or not
     */
    bool IsClosing();
    
    /**
     * Platform-specific event loop
     */
    void ProcessEvents();
    
    /**
     * Swap window buffers
     */
    void SwapBuffer();
    
protected:
    // Internal window handle
    void* m_window;
    
    // Whether our platform specific properties have been initialized
    bool m_initialized;
    
    // Window size
    int m_width, m_height;
    
    // Framebuffer size
    int m_framebufferWidth, m_framebufferHeight;
    
    // Fullscreen?
    bool m_fullscreen;
    
    // Window name
    std::string m_name;
};

#endif
