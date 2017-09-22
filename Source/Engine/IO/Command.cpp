
#include <giga-engine.h>

std::vector<Command::CommandType*> Command::m_commandTypes;

Command::Command() {
	type = 0;
	start = 0;
	end = 0;
	entityID = 0;
}

void Command::RegisterCommandType(std::string name, int typeID) {
	if (m_commandTypes.size()) {
		for (int i = m_commandTypes.size() - 1; i >= 0; i--) {
			if (m_commandTypes[i]->name == name || m_commandTypes[i]->typeID == typeID) {
				GIGA_ASSERT(false, "Command type name or typeID already registered.");
				return;
			}
		}
	}

	CommandType* type = new CommandType;
	type->name = name;
	type->typeID = typeID;
	m_commandTypes.push_back(type);
}

void Command::Start() {
	NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
	int tick = networkSystem->GetCurrentTick();
	tick -= NETWORK_SNAPSHOT_RENDER_LAG;

	start = tick;
	EventSystem::Process(new Event("COMMAND_START", this, entityID));
}

void Command::End() {
	NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
	int tick = networkSystem->GetCurrentTick();
	tick -= NETWORK_SNAPSHOT_RENDER_LAG;

	end = tick;
	EventSystem::Process(new Event("COMMAND_END", this, entityID));
}

void Command::Initialize(Variant** argv, int argc) {
	if (argc == 0) return;

	GIGA_ASSERT(argc == 2, "Expecting two arguments.");
	GIGA_ASSERT(argv[0]->IsInt(), "Expecting first parameter to be an entity ID.");
	GIGA_ASSERT(argv[1]->IsString(), "Expecting parameter one to be a string command type.");

	for (size_t i = 0; i < m_commandTypes.size(); i++) {
		if (m_commandTypes[i]->name == argv[1]->AsString()) {
			type = m_commandTypes[i]->typeID;
			break;
		}
	}

	GIGA_ASSERT(type > 0, "Unregistered command type.");

	EntitySystem* entitySystem = GetSystem<EntitySystem>();
	Entity* entity = entitySystem->FindEntity(argv[0]->AsInt());
	entityID = entity->GetID();
}

Variant* Command::RegisterCommandType(Variant* object, int argc, Variant** argv) {
	GIGA_ASSERT(argc == 2, "RegisterCommandType expects two arguments.");
	GIGA_ASSERT(argv[0]->IsString(), "First parameter should be string command name.");
	GIGA_ASSERT(argv[1]->IsInt(), "Second parameter should be integer type ID.");

	Command::RegisterCommandType(argv[0]->AsString(), argv[1]->AsInt());

	return(new Variant(0));
}

Variant* Command::Start(Variant* object, int argc, Variant** argv) {
	GIGA_ASSERT(argc == 0, "Start expects no arguments.");

	Command* command = object->AsObject<Command>();
	command->Start();
	return(new Variant(0));
}

Variant* Command::End(Variant* object, int argc, Variant** argv) {
	GIGA_ASSERT(argc == 0, "End expects no arguments.");

	Command* command = object->AsObject<Command>();
	command->End();
	return(new Variant(0));
}

Variant* Command::Type(std::string prop, Variant* obj) {
	Command* command = obj->AsObject<Command>();
	for (size_t i = 0; i < m_commandTypes.size(); i++) {
		if (m_commandTypes[i]->typeID == command->type) {
			return(new Variant(m_commandTypes[i]->name));
		}
	}

	GIGA_ASSERT(false, "Unknown command type.");
	return(new Variant(0));
}
