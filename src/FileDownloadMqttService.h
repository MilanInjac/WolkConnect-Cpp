/*
 * Copyright 2017 WolkAbout Technology s.r.o.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FILEDOWNLOADMQTTSERVICE_H
#define FILEDOWNLOADMQTTSERVICE_H

#include "CommandHandlingService.h"
#include "BinaryDataListener.h"
#include "FileDownloadMqttCommandListener.h"
#include "FileHandler.h"
#include "model/FileDownloadMqttResponse.h"
#include <memory>
#include <functional>

namespace wolkabout
{
class OutboundServiceDataHandler;

class FileDownloadMqttService: public CommandHandlingService, public FileDownloadMqttCommandListener, public BinaryDataListener
{
public:
	FileDownloadMqttService(std::shared_ptr<OutboundServiceDataHandler> outboundDataHandler);

	void handleBinaryData(const BinaryData& binaryData) override;

	void handleFileDownloadMqttCommand(const FileDownloadMqttCommand& fileDownloadMqttCommand) override;

	void setFileDownloadedCallback(std::function<void(const std::string&)> callback);

	void abortDownload();

private:
	void sendResponse(const FileDownloadMqttResponse& response);

	enum class State
	{
		IDLE = 0,
		DOWNLOAD
	};

	FileDownloadMqttService::State m_currentState;
	std::shared_ptr<OutboundServiceDataHandler> m_outboundDataHandler;
	std::unique_ptr<FileHandler> m_fileHandler;
	std::string m_fileName;

	FileDownloadMqttResponse m_lastResponse;

	std::function<void(const std::string&)> m_fileDownloadedCallback;
};
}

#endif // FILEDOWNLOADMQTTSERVICE_H