#ifndef CUSTOMHANDLER_H
#define CUSTOMHANDLER_H

#include <QtCore/QObject>
#include "ActuationHandler.h"
#include "ActuatorStatusProvider.h"
#include "BasicConfigLoader.h"
#include <atomic>

namespace example
{

class CustomHandler: public QObject, public wolkabout::ActuationHandler, public wolkabout::ActuatorStatusProvider
{
Q_OBJECT
public:
	void handleActuation(const std::string& reference, const std::string& value) override
	{
		if(reference == "MSG")
		{
			if(BasicConfigLoader::saveMessage("config", value))
			{
				m_message = value;
				m_messageStatus = wolkabout::ActuatorStatus::State::BUSY;
				emit message(value);
			}
			else
			{
				m_messageStatus = wolkabout::ActuatorStatus::State::ERROR;
			}
		}
		else if(reference == "STATE")
		{
			if(value == "true")
			{
				m_toggle = true;
			}
			else
			{
				m_toggle = false;
			}

			m_toggleStatus = wolkabout::ActuatorStatus::State::BUSY;
			emit toggle(m_toggle);
		}
	}

	wolkabout::ActuatorStatus getActuatorStatus(const std::string& reference) override
	{
		if (reference == "MSG")
		{
			return wolkabout::ActuatorStatus(m_message, m_messageStatus);
		}
		else if(reference == "STATE")
		{
			return wolkabout::ActuatorStatus(m_toggle ? "true" : "false", m_toggleStatus);
		}

		return wolkabout::ActuatorStatus("", wolkabout::ActuatorStatus::State::READY);
	}

signals:
	void message(std::string message);
	void toggle(bool tog);

	void statusUpdated(std::string ref, wolkabout::ActuatorStatus status);

public slots:
	void messageSetSuccess(bool status)
	{
		if(status)
		{
			m_messageStatus = wolkabout::ActuatorStatus::State::READY;
		}
		else
		{
			m_messageStatus = wolkabout::ActuatorStatus::State::ERROR;
		}

		emit statusUpdated("MSG", {m_message, m_messageStatus});
	}

	void toggleSetSuccess(bool status, bool value)
	{
		if(status)
		{
			m_toggleStatus = wolkabout::ActuatorStatus::State::READY;
		}
		else
		{
			m_toggleStatus = wolkabout::ActuatorStatus::State::ERROR;
		}

		m_toggle = value;

		emit statusUpdated("STATE", {m_toggle ? "true" : "false", m_toggleStatus});
	}

private:
	std::atomic<wolkabout::ActuatorStatus::State> m_messageStatus;
	std::string m_message;
	std::atomic<wolkabout::ActuatorStatus::State> m_toggleStatus;
	std::atomic_bool m_toggle;
};

}

#endif
