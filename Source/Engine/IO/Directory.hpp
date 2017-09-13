
#ifndef directory_hpp
#define directory_hpp

class Directory {
public:
    Directory();
    ~Directory() = default;
    
    /**
     * Get current working directory of application
     */
    static std::string GetCurrentDirectory();
};

#endif
