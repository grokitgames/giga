
#include <giga-engine.h>

CommandMessage::CommandMessage() {
	m_command = 0;
	m_envelope.flags |= NetworkMessage::Flags::FLAG_ACK;
	m_envelope.type = 40;
}

void CommandMessage::OnSend() {
	GIGA_ASSERT(m_command != 0, "Command object not set.");

	unsigned char* message = (unsigned char*)malloc(sizeof(uint32_t) * 6);

	int offset = 0;
	memcpy(message + offset, &m_command->type, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(message + offset, &m_command->entityID, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(message + offset, &m_command->start, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(message + offset, &m_command->end, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(message + offset, &m_command->sessionID, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	memcpy(message + offset, &m_command->commandID, sizeof(uint32_t));
	offset += sizeof(uint32_t);

	SetPayload(message, sizeof(uint32_t) * 6);
}

void CommandMessage::OnReceive() {
	MemoryReader* reader = new MemoryReader();
	reader->Initialize(m_payload, m_envelope.bytes);

	uint32_t type, entityID, start, end, sessionID, commandID;
	reader->Read(&type, sizeof(uint32_t));
	reader->Read(&entityID, sizeof(uint32_t));
	reader->Read(&start, sizeof(uint32_t));
	reader->Read(&end, sizeof(uint32_t));
	reader->Read(&sessionID, sizeof(uint32_t));
	reader->Read(&commandID, sizeof(uint32_t));

	// Save the original start and end difference
	int diff = end - start;

	NetworkSystem* networkSystem = GetSystem<NetworkSystem>();
	NetworkSession* session = networkSystem->FindSession(m_envelope.session);
	int tick = networkSystem->GetCurrentTick();

	if (end == 0) {
		start = tick - floor(NETWORK_TICKS_PER_SECOND * session->info.pingTime) - NETWORK_SNAPSHOT_RENDER_LAG;
		printf("Current tick: %d, setting start to %d\n", tick, start);
	}
	else {
		end = tick - floor(NETWORK_TICKS_PER_SECOND * session->info.pingTime) - NETWORK_SNAPSHOT_RENDER_LAG;
		printf("Current tick: %d, setting end to %d\n", tick, end);
	}

	delete reader;

	// Make sure the replication system has been run for this tick
	ReplicationSystem* replicationSystem = GetSystem<ReplicationSystem>();

	// If this is a start command, make sure the end hasn't already been received
	if (end == 0) {
		// Find the start
		Command* endCmd = replicationSystem->GetCommand(entityID, commandID);
		if (endCmd) {
			return;
		}
	}
	else {
		// If this is an end command and we don't have a start command, insert it twice
		Command* startCmd = replicationSystem->GetCommand(entityID, commandID);
		if (startCmd == 0) {
			Command* command = new Command();
			command->type = type;
			command->entityID = entityID;
			command->start = end - 1;
			command->end = 0;
			command->commandID = commandID;
			command->sessionID = m_envelope.session;

			replicationSystem->AddCommand(command);
		}
	}

	// Save command
	Command* command = new Command();
	command->type = type;
	command->entityID = entityID;
	command->start = start;
	command->end = end;
	command->commandID = commandID;
	command->sessionID = m_envelope.session;

	replicationSystem->AddCommand(command);
}

void CommandMessage::Initialize(Variant** argv, int argc) {
	if (argc == 0) return;

	GIGA_ASSERT(argc == 1, "CommandMessage expects a single argument.");
	GIGA_ASSERT(argv[0]->IsObject(), "Expecting first parameter to be a command object.");

	m_command = argv[0]->AsObject<Command>();
}