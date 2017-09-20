
#ifndef file_hpp
#define file_hpp

/**
 * File opening modes (can/should be OR'd together)
 */
enum {
    FILEMODE_READ = 1,
    FILEMODE_READWRITE = 2,
    FILEMODE_BINARY = 4,
    FILEMODE_APPEND = 8,
};

/**
 * A low level file access class
 */
class GIGA_API File : public ScriptableObject {
public:
    File();
    File(std::string filename, int mode);
    ~File();

	GIGA_CLASS_NAME("File");
    
    /**
     * Open a file
     */
    bool Open(std::string filename, int mode);
    
    /**
     * Check whether a file exists
     */
    bool Exists(std::string filename);
    
    /**
     * Close the current file
     */
    void Close();
    
    /**
     * Whether we have reached the end of the file yet
     */
    bool EndOfFile();
    
    /**
     * Get the file size
     */
    unsigned long GetFileSize();
    
    /**
     * Read from the file
     */
    unsigned char* Read(unsigned long bytes);
    
    /**
     * Read from a file - dest must be initialized already
     */
    void Read(unsigned char* dest, unsigned long bytes);
    
    /**
     * Write to file
     */
    void Write(unsigned char* data, unsigned long bytes);
    
    /**
     * Write a single line (+ newline) to file
     */
    void WriteLine(std::string line);
    
    /**
     * Get the entire contents of a file
     */
    unsigned char* ReadFile();
    
    /**
     * Go to a specific position in the file
     */
    void SetPosition(unsigned int offset);

	/**
	* Scripting integration
	*/
	static Variant* Load(Variant* object, int argc, Variant** argv);
    
protected:
    // File handle
    FILE* m_fp;
    
    // Name of the open file
    std::string m_filename;
    
    // Current file mode
    int m_mode;
    
    // End of file?
    bool m_eof;
};

#endif
