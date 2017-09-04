
#include <giga-engine.h>

EntitySnapshot::~EntitySnapshot() {
    for(size_t i = 0; i < entities.size(); i++) {
        delete entities[i];
    }
}

void EntitySnapshot::Serialize(unsigned char* buffer, int& bufferSize, int& offset) {
    // Keep track of our current buffersize in a memory writer
    MemoryWriter* writer = new MemoryWriter();
    writer->Initialize(buffer, bufferSize);

	// Write type
	uint32_t t = type;
	writer->Write(&t, sizeof(uint32_t));
    
    // Iterate over entities and serialize their components
    for(size_t i = offset; i < entities.size(); i++) {
        if(writer->GetPosition() + sizeof(uint32_t) >= bufferSize) {
            break;
        }
        
        // Get number of components
        uint32_t componentCount = 0;
        int projectedSize = writer->GetPosition() + sizeof(uint32_t);
        
        // Iterate over components to get total size
        std::vector<Component*> components = entities[i]->GetComponents();
        for(size_t j = 0; j < components.size(); j++) {
			components[j]->SetDataMappings();
			int size = components[j]->GetSerializedSize();
            projectedSize += size + sizeof(uint32_t);
        }
        
        // If this entity won't fit, return
        if(projectedSize >= bufferSize) {
            break;
        }
        
        // Write entity ID
        uint32_t id = entities[i]->GetID();
        writer->Write(&id, sizeof(uint32_t));

		// Write number of components
		uint32_t numComponents = components.size();
		writer->Write(&numComponents, sizeof(uint32_t));
        
        // Write components
        for(size_t j = 0; j < components.size(); j++) {
            int size = 0;
            unsigned char* data = components[j]->Serialize(size);
            
            uint32_t type = components[j]->GetTypeID();
            writer->Write(&type, sizeof(uint32_t));
            writer->Write(data, size);
            
            free(data);
        }
        
        offset++;
    }
    
    bufferSize = writer->GetPosition();
    delete writer;
}

void EntitySnapshot::Deserialize(unsigned char* buffer, int bufferSize) {
    // Deserialize buffer, start with reading data out
    MemoryReader* reader = new MemoryReader();
    reader->Initialize(buffer, bufferSize);
    
    // Get a link to our entity system
    EntitySystem* entitySystem = GetSystem<EntitySystem>();

	// Read type
	uint32_t t = type;
	reader->Read(&t, sizeof(uint32_t));
	type = t;
    
    int offset = 0;
    while(offset < bufferSize) {
        // First, entity ID
        uint32_t entityID = 0;
        reader->Read(&entityID, sizeof(uint32_t));
        
        // Create a new entity
        Entity* entity = new Entity();
        entity->SetID(entityID);
        
        // Then number of components
        uint32_t numComponents = 0;
        reader->Read(&numComponents, sizeof(uint32_t));
        
        for(int i = 0; i < numComponents; i++) {
            // Read component type
            uint32_t typeID = 0;
            reader->Read(&typeID, sizeof(uint32_t));
            
            // Create a component
            Component* component = Component::CreateComponent(typeID);
            component->SetDataMappings();
            
            // Get our data block
            component->Deserialize(reader);
			
            // Add to our entity
            entity->AddComponent(component);
        }
        
        offset = reader->GetPosition();

		entities.push_back(entity);
    }
    
    delete reader;
}
