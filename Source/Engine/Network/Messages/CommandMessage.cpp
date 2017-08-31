
#include <giga-engine.h>

void CommandMessage::OnSend() {
	m_envelope.type = 40;

	GIGA_ASSERT(m_command != 0, "Command object not set.");

	unsigned char* message = (unsigned char*)malloc(sizeof(uint32_t) * 4);

	int offset = 0;
	memcpy(message + offset, &m_command->type, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(message + offset, &m_command->entityID, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(message + offset, &m_command->start, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(message + offset, &m_command->end, sizeof(uint32_t));

	SetPayload(message, sizeof(uint32_t) * 4);
}

void CommandMessage::OnReceive() {
	MemoryReader* reader = new MemoryReader();
	reader->Initialize(m_payload, m_envelope.bytes);

	uint32_t type, entityID, start, end;
	reader->Read(&type, sizeof(uint32_t));
	reader->Read(&entityID, sizeof(uint32_t));
	reader->Read(&start, sizeof(uint32_t));
	reader->Read(&end, sizeof(uint32_t));

	delete reader;

	int redoTick = (end > 0) ? end : start;
	std::string eventStr = (end > 0) ? "COMMAND_END" : "COMMAND_START";

	printf("Command: %s\n", eventStr.c_str());

	// Do things
	EntitySystem* entitySystem = GetSystem<EntitySystem>();
	
	// Roll system state back to previous tick when event occured
	NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
	int tick = networkSystem->GetCurrentTick();

	// Reset back to previous tick
	entitySystem->Clear();

	ReplicationSystem* replicationSystem = GetSystem<ReplicationSystem>();
	EntitySnapshot* snapshot = replicationSystem->GetFullEntitySnapshot(redoTick);
	for (size_t i = 0; i < snapshot->entities.size(); i++) {
		entitySystem->AddEntity(snapshot->entities[i]);
	}

	// Get our entity
	Entity* entity = entitySystem->FindEntity(entityID);
	GIGA_ASSERT(entity != 0, "Entity not found.");

	// Execute event
	Command* command = new Command();
	command->type = type;
	command->entityID = entityID;
	command->start = start;
	command->end = end;

	EventSystem::Process(new Event(eventStr, command, entity));

	// "Replay" ticks over time
	Application* application = Application::GetInstance();
	for (int i = redoTick; i <= tick; i++) {
		float delta = (1.0f / NETWORK_TICKS_PER_SECOND);
		application->Update(delta);

		printf("Replaying tick %i with delta %f.\n", i, delta);
	}

	delete command;
}

void CommandMessage::Initialize(Variant** argv, int argc) {
	if (argc == 0) return;

	GIGA_ASSERT(argc == 1, "CommandMessage expects a single argument.");
	GIGA_ASSERT(argv[0]->IsObject(), "Expecting first parameter to be a command object.");

	m_command = argv[0]->AsObject<Command>();
}