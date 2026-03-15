#pragma once
#include <QObject>

class PriorityEnum {
	Q_GADGET
public:
	enum Priority {
		EMERGENCY,
		ALERT,
		CRITICAL,
		ERROR,
		WARNING,
		NOTICE,
		INFO,
		DEBUG
	};
	Q_ENUM(Priority)
};
