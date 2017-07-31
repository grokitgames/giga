
#include <giga-engine.h>

Profiler* Profiler::m_instance = 0;

Profiler::Profiler() {
    m_currentArea = 0;
    m_rootArea = 0;
    m_timer = 0;
    m_maxAreaNameLength = 0;
    m_currentDepth = 0;
}

Profiler::~Profiler() {
    if (m_rootArea) {
        delete m_rootArea;
    }
    
    if (m_timer) {
        delete m_timer;
    }
}

Profiler::ProfileArea::ProfileArea() {
    avgFrameTime = 0;
    currentStartTime = 0;
    parent = 0;
}

Profiler::ProfileArea::~ProfileArea() {
    std::map<std::string, ProfileArea*>::iterator i = children.begin();
    for (i; i != children.end(); i++) {
        delete i->second;
    }
}

Profiler* Profiler::GetInstance() {
    if (m_instance == 0) {
        m_instance = new Profiler();
    }
    
    return(m_instance);
}

void Profiler::Log() {
    std::string headerFn = "Function Name";
    std::string headerAvg = "Duration";
    m_maxAreaNameLength = std::max(m_maxAreaNameLength, (int)headerFn.length());
    
    char buffer[1000];
    int level = 1;
    sprintf(buffer, "% *c%- *s | %- 10s ms", level * 2, ' ', m_maxAreaNameLength - (level * 2), headerFn.c_str(), headerAvg.c_str());
    Application::Log(ERROR_DEBUG, buffer);
    
    RecursiveLog(level, m_rootArea);
}

void Profiler::RecursiveLog(int level, Profiler::ProfileArea* area) {
    char buffer[1000];
    sprintf(buffer, "% *c%- *s | % 10.4f ms", level * 2, ' ', m_maxAreaNameLength - (level * 2), area->areaName.c_str(), area->avgFrameTime);
    Application::Log(ERROR_DEBUG, buffer);
    
    std::map<std::string, ProfileArea*>::iterator i = area->children.begin();
    for (i; i != area->children.end(); i++) {
        RecursiveLog(level + 1, i->second);
    }
}

void Profiler::StartFrame() {
    // Make sure we have a root node
    if (m_rootArea == 0) {
        m_rootArea = new ProfileArea();
        m_rootArea->areaName = "root";
    }
    
    // Set it to our current node
    m_currentArea = m_rootArea;
    m_currentDepth = 0;
    
    // Get start time
    if (m_timer == 0) {
        m_timer = new Timer();
    }
    
    m_timer->Start();
}

void Profiler::EndFrame() {
    m_currentArea = 0;
    m_currentDepth = 0;
    
    m_timer->Reset();
}

void Profiler::StartArea(std::string name) {
    assert(m_currentArea != 0); // Must start a frame first
    
    m_currentDepth++;
    m_maxAreaNameLength = std::max(m_maxAreaNameLength, (int)name.length() + (2 * m_currentDepth) + 2);
    
    std::map<std::string, ProfileArea*>::iterator child = m_currentArea->children.find(name);
    if (child == m_currentArea->children.end()) {
        ProfileArea* area = new ProfileArea();
        area->areaName = name;
        m_currentArea->children[name] = area;
        area->parent = m_currentArea;
        
        m_currentArea = area;
    }
    else {
        m_currentArea = child->second;
    }
    
    m_currentArea->currentStartTime = m_timer->Duration();
}

void Profiler::EndArea(std::string name) {
    if (m_currentArea->areaName != name) {
        printf("Current area name: %s\n", m_currentArea->areaName.c_str());
        assert(false); // You forgot to close a profiling section
    }
    
    float currentOffset = m_timer->Duration();
    float timeDiff = (currentOffset - m_currentArea->currentStartTime) * 1000.0f;
    
    if (m_currentArea->avgFrameTime) {
        m_currentArea->avgFrameTime = (m_currentArea->avgFrameTime + timeDiff) / 2.0f;
    }
    else {
        m_currentArea->avgFrameTime = timeDiff;
    }
    
    if (m_currentArea->parent) {
        m_currentArea = m_currentArea->parent;
    }
    else {
        m_currentArea = m_rootArea;
    }
    
    m_currentDepth--;
}
