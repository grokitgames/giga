
#ifndef clientpredictionerror_hpp
#define clientpredictionerror_hpp

/**
* Represents a difference in position or rotation and the
*/
class GIGA_API ClientPredictionError {
public:
	ClientPredictionError() = default;
	~ClientPredictionError() = default;

public:
	int startTick;
	int endTick;
	int entityID;
	int componentTypeID;
};

#endif