
#include <giga-engine.h>

Resource::Resource() {
    m_data = 0;
    m_lastAccessTime = 0;
    m_loaded = false;
    m_filesize = 0;
}

Resource::~Resource() {
    if(m_data) {
        free(m_data);
    }
}

void Resource::SetFilename(std::string filename) {
    // Set the full path to the filename
    m_fullpath = filename;
    
    // Parse out the filename
    size_t loc = m_fullpath.find_last_of("/\\");
    this->filename = m_fullpath.substr(loc + 1);
    
    // And the file extension
    loc = m_fullpath.find_last_of(".");
    m_extension = m_fullpath.substr(loc + 1);
}

unsigned char* Resource::GetData() {
    // If this file is not yet loaded, load it
    if(m_loaded == false) {
        Load();
    }
    
    // Set the last access time
    DateTime* current = DateTime::GetCurrent();
    m_lastAccessTime = current->GetTimestamp();
    delete current;
    
    // Return the data
    return(m_data);
}

void Resource::Load() {
    // Make sure we have a valid filename
    assert(this->filename.length() > 0);
    
    // If this is already loaded, return
    if(m_data) {
        return;
    }
    
    // Try to open the file
    File* fp = new File();
    if(fp->Open(m_fullpath, FILEMODE_READ | FILEMODE_BINARY) == false) {
        ErrorSystem::Process(new Error(ERROR_WARN, "Unable to open resource file", this->filename));
        return;
    }
    
    // Get the file size
    m_filesize = fp->GetFileSize();
    
    // Load the contents of the file
    m_data = fp->ReadFile();
    
    // Close the file
    fp->Close();
    
    m_loaded = true;
}

void Resource::Unload() {
    // If we have data loaded, discard it
    if(m_data) {
        free(m_data);
        m_data = 0;
    }
    
    m_loaded = false;
}
