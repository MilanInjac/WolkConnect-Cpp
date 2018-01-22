#ifndef BASICURLFILEDOWNLOADER_H
#define BASICURLFILEDOWNLOADER_H

#include "UrlFileDownloader.h"
#include "utilities/FileSystemUtils.h"
#include "utilities/ByteUtils.h"

namespace example
{
class BasicUrlFileDownloader: public wolkabout::UrlFileDownloader
{
public:
	void download(const std::string& url, const std::string& downloadDirectory,
				  std::function<void(const std::string& filePath)> onSuccessCallback,
				  std::function<void(UrlFileDownloader::Error errorCode)> onFailCallback) override
	{
		if(wolkabout::FileSystemUtils::isFilePresent(url))
		{
			wolkabout::ByteArray content;
			if(wolkabout::FileSystemUtils::readBinaryFileContent(url, content))
			{
				const std::string filePath = downloadDirectory + "/new_firmware_file";
				if(wolkabout::FileSystemUtils::createBinaryFileWithContent(filePath, content))
				{
					onSuccessCallback(filePath);
					return;
				}
			}
		}

		onFailCallback(Error::UNSPECIFIED_ERROR);
	}

	void abort() override
	{
	}
};
}

#endif
