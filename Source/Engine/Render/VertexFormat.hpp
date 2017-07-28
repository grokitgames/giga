
#ifndef vertexformat_hpp
#define vertexformat_hpp

/**
 * Vertex layout definition
 */
enum {
    VERTEX_HAS_POSITION = 0x01,
    VERTEX_HAS_COLOR = 0x02,
    VERTEX_HAS_NORMAL = 0x04,
    VERTEX_HAS_TEXCOORD0 = 0x08,
    VERTEX_HAS_TEXCOORD1 = 0x10,
    VERTEX_HAS_BONES = 0x20,
};

enum {
    VERTEX_ATTRIB_POSITION = 0,
    VERTEX_ATTRIB_COLOR = 1,
    VERTEX_ATTRIB_NORMAL = 2,
    VERTEX_ATTRIB_TEXCOORD0 = 3,
    VERTEX_ATTRIB_TEXCOORD1 = 4,
    VERTEX_ATTRIB_BONES = 5,
    VERTEX_ATTRIB_BONE_WEIGHTS = 6,
    VERTEX_ATTRIB_LAST,
};

/**
 * The format of our vertices for an object
 */
class GIGA_API VertexFormat {
public:
    VertexFormat();
    ~VertexFormat();
    
    struct VertexAttrib {
        unsigned int index;
        unsigned int components;
        unsigned int stride;
        unsigned int offset;
    };
    
    void AddVertexAttribute(int index, int components, int stride, int offset);
    void EnableVertexAttribs(int attribs);
    void DisableVertexAttribs();
    
    void Use();
    
protected:
    std::vector<VertexAttrib*> m_attribs;
    
    // Vertex array object (GL3+ state for buffers)
    VertexAttribObject* m_vertexArrayObject;
};

#endif
