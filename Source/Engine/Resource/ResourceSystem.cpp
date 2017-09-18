
#include <giga-engine.h>

ResourceSystem::~ResourceSystem() {
    
}

void ResourceSystem::AddResource(ResourceObject *resource) {
    m_resources.AddObject(resource);
}

void ResourceSystem::RemoveResource(ResourceObject* resource) {
    m_resources.RemoveObject(resource);
}

void ResourceSystem::Initialize() {

}

ResourceObject* ResourceSystem::LoadResource(std::string filename, std::string type) {
    // Check to see if we've loaded this resource
    ResourceObject* resource = this->Find(filename);
    
    // If not, try to find and load it
    if(resource == 0) {
        for(size_t i = 0; i < m_resourceTypes.size(); i++) {
            if(m_resourceTypes[i]->name == type) {
                resource = m_resourceTypes[i]->createFunc();
                break;
            }
        }
        
        GIGA_ASSERT(resource != 0, "Resource type not found.");
        
        // First, get the full path by searching out search paths
        std::string fullpath = FindResourcePath(filename);
        if(fullpath.empty()) {
            ErrorSystem::Process(new Error(ERROR_WARN, (char*)"Unable to locate resource file", filename));
            return(0);
        }
        
        // Create a resource and load
        Resource* rs = new Resource();
		rs->SetType(type);
        rs->SetFilename(fullpath);
        
        rs->Load();
        
        resource->m_resource = rs;
        resource->ProcessData();
        
        AddResource(resource);
    }
    
    return(resource);
}

ResourceObject* ResourceSystem::Find(std::string name) {
    // See if we've already loaded it
    std::list<ResourceObject*> resources = m_resources.GetList();
	std::list<ResourceObject*>::iterator i = resources.begin();
	for (i; i != resources.end(); i++) {
        if((*i)->m_resource->GetFilename() == name) {
            return((*i));
        }
    }
    
    return(0);
}

void ResourceSystem::AddSearchPath(std::string path) {
    m_paths.push_back(path);
}

std::string ResourceSystem::FindResourcePath(std::string filename) {
    std::string ret;
    
    // First try to open the file with the path it's on now
    File* f = new File;
    if(f->Open(filename, FILEMODE_READ | FILEMODE_BINARY)) {
        f->Close();
        
        ret = filename;
        delete f;
        return(ret);
    }
    delete f;
    
    // Loop through our list of files, try to open this one
    std::vector<std::string>::iterator i = m_paths.begin();
    for(; i != m_paths.end(); i++) {
        File* f = new File;
        std::string fn(*i);
        fn += "/";
        fn += filename;
        
        if(f->Open(fn, FILEMODE_READ | FILEMODE_BINARY)) {
            f->Close();
            
            delete f;
            return(fn);
        }
        
        f->Close();
        delete f;
    }
    
    return(ret);
}

void ResourceSystem::Update(double delta) {
    // Get current timestamp
    DateTime* current = DateTime::GetCurrent();
    time_t timestamp = current->GetTimestamp();
    
	std::list<ResourceObject*> resources = m_resources.GetList();
	std::list<ResourceObject*>::iterator i = resources.begin();
	for (i; i != resources.end(); i++) {
        Resource* resource = (*i)->m_resource;
        if(timestamp - resource->GetLastAccess() > RESOURCE_DELETION_TIME) {
            resource->Unload();
        }
    }
    
    delete current;
}

Variant* ResourceSystem::LoadResource(Variant* object, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 1, "LoadResource expects two arguments.");
    GIGA_ASSERT(argv[0]->IsString(), "First parameter should be a filename string.");
    GIGA_ASSERT(argv[0]->IsString(), "Second parameter should be a class name string.");
    
    ResourceSystem* resourceSystem = GetSystem<ResourceSystem>();
    return(new Variant(resourceSystem->LoadResource(argv[0]->AsString(), argv[1]->AsString())));
}
