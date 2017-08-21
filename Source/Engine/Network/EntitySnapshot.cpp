
#include <giga-engine.h>

void EntitySnapshot::Serialize(unsigned char* buffer, int& bufferSize, int& offset) {
    // Keep track of our current buffersize in a memory writer
    MemoryWriter* writer = new MemoryWriter();
    writer->Initialize(buffer, bufferSize);
    
    // Iterate over entities and serialize their components
    for(size_t i = offset; i < entities.size(); i++) {
        if(writer->GetPosition() + sizeof(uint32_t) >= bufferSize) {
            break;
        }
        
        // Get number of components
        uint32_t componentCount = 0;
        int projectedSize = writer->GetPosition() + sizeof(uint32_t);
        
        // Iterate over components to get total size
        std::vector<Component*>& components = entities[i]->GetComponents();
        for(size_t j = 0; j < components.size(); j++) {
			components[j]->UpdateObjectDataMappings();
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

		printf("Entity ID %d\n", id);

		// Write number of components
		uint32_t numComponents = components.size();
		writer->Write(&numComponents, sizeof(uint32_t));

		printf("Component count %d\n", numComponents);
        
        // Write components
        for(size_t j = 0; j < components.size(); j++) {
            int size = 0;
            unsigned char* data = components[j]->Serialize(size);
            
            uint32_t type = components[j]->GetTypeID();
			printf("Writing component type %d\n", type);
            writer->Write(&type, sizeof(uint32_t));
            writer->Write(data, size);
        }
        
        offset++;
    }
    
    delete writer;
    bufferSize = writer->GetPosition();
}

void EntitySnapshot::Deserialize(unsigned char* buffer, int bufferSize) {
    // Deserialize buffer, start with reading data out
    MemoryReader* reader = new MemoryReader();
    reader->Initialize(buffer, bufferSize);
    
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
            
            // Get our data block
			component->UpdateObjectDataMappings();
            component->Deserialize(reader);
			
            // Add to our entity
            entity->AddComponent(component);
        }
        
        offset = reader->GetPosition();
    }
}
