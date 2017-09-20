
#include <giga-engine.h>

File::File() {
    m_fp = 0;
    m_mode = 0;
    m_eof = false;
}

File::File(std::string filename, int mode) {
    m_fp = 0;
    m_mode = 0;
    m_eof = false;
    
    Open(filename, mode);
}

File::~File() {
    if (m_fp) {
        fclose(m_fp);
        m_fp = 0;
    }
}

bool File::Open(std::string filename, int mode) {
    if (m_fp) {
        fclose(m_fp);
        m_fp = 0;
    }
    
    m_filename = filename;
    
    std::string flags = "";
    if (mode & FILEMODE_READWRITE) {
        flags += "w";
    }
    if (mode & FILEMODE_READ) {
        flags += "r";
    }
    if (mode & FILEMODE_APPEND) {
        flags += "a";
    }
    if (mode & FILEMODE_BINARY) {
        flags += "b";
    }
    
    m_fp = fopen(m_filename.c_str(), flags.c_str());
    if (m_fp == 0) {
        return(false);
    }
    
    m_mode = mode;
    
    return(true);
}

bool File::Exists(std::string filename) {
    FILE* fp = 0;
    fp = fopen(filename.c_str(), "rb");
    
    if (fp == 0) {
        return(false);
    }
    
    fclose(fp);
    
    return(true);
}

bool File::EndOfFile() {
    return(m_eof);
}

void File::Close() {
    if (m_fp) {
        fclose(m_fp);
        m_fp = 0;
    }
}

void File::Write(unsigned char *data, unsigned long bytes) {
    if (m_fp == 0) {
        return;
    }
    
    unsigned int written = (unsigned int)fwrite(data, 1, bytes, m_fp);
    fflush(m_fp);
}

void File::WriteLine(std::string line) {
    if (m_fp == 0) {
        return;
    }
    
    Write((unsigned char*)line.c_str(), (unsigned long)line.length());
    Write((unsigned char*)"\n", 1);
}

unsigned char* File::Read(unsigned long bytes) {
    if (m_fp == 0) {
        return(0);
    }
    
    unsigned char* ret = (unsigned char*)malloc(bytes + (m_mode & FILEMODE_BINARY ? 0 : 1));
    size_t actual = fread(ret, 1, bytes, m_fp);
    
    if (actual == 0) {
        free(ret);
        m_eof = true;
        return(0);
    }
    
    if ((m_mode & FILEMODE_BINARY) == false) {
        ret[actual] = '\0';
    }
    
    return(ret);
}

void File::Read(unsigned char* dest, unsigned long bytes) {
    if (m_fp == 0) {
        return;
    }
    
    unsigned long read = (unsigned int)fread(dest, 1, bytes, m_fp);
    
    if (read == 0) {
        m_eof = true;
    }
    
    return;
}

unsigned char* File::ReadFile() {
    if (m_fp == 0) {
        return(0);
    }
    
    unsigned long filesize = GetFileSize();
    return(Read(filesize));
}

unsigned long File::GetFileSize() {
    if (m_fp == 0) {
        return(0);
    }
    
    unsigned long pos = ftell(m_fp);
    fseek(m_fp, 0, SEEK_END);
    unsigned long filesize = ftell(m_fp);
    fseek(m_fp, pos, SEEK_SET);
    
    return(filesize);
}

void File::SetPosition(unsigned int offset) {
    fseek(m_fp, SEEK_SET, offset);
}

Variant* File::Load(Variant* object, int argc, Variant** argv) {
	GIGA_ASSERT(argc == 1, "Load expects one argument.");
	GIGA_ASSERT(argv[0]->IsString(), "First argument should be string file name.");

	// Attempt to get full path
	ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
	std::string fullPath = resourceSystem->FindResourcePath(argv[0]->AsString());

	if (fullPath.length() <= 0) {
		return(new Variant(0));
	}

	File* file = new File();
	file->Open(fullPath, FILEMODE_READ);
	std::string data = (char*)file->ReadFile();
	file->Close();

	return(new Variant(data));
}