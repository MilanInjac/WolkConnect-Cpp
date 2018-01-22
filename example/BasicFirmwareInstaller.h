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

#ifndef BASICFIRMWAREINSTALLER_H
#define BASICFIRMWAREINSTALLER_H

#include "FirmwareInstaller.h"
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/stat.h>

namespace example
{
class BasicFirmwareInstaller: public wolkabout::FirmwareInstaller
{
public:
	BasicFirmwareInstaller(int argc, char** argv, char** envp) : m_argc{argc}, m_argv{argv}, m_envp{envp}
	{
	}

	bool install(const std::string& firmwareFile) override
	{
		std::cout << "Install file " << firmwareFile << std::endl;

		std::cout << m_argv[0];

		unlink(m_argv[0]);

		std::string newExe = std::string(m_argv[0]) + "_dfu";

		std::rename(firmwareFile.c_str(), newExe.c_str());

		chmod(newExe.c_str(), S_IRWXU|S_IRWXG|S_IRWXO);

		char * argv[] = {(char*)newExe.c_str(), nullptr};
		char * envp[] = {nullptr};

		auto ret = execve(newExe.c_str(), argv, m_envp);

		std::cout << std::strerror(errno) << std::endl;

		return ret != -1;
	}

private:
	int m_argc;
	char** m_argv;
	char** m_envp;
};
}

#endif // BASICFIRMWAREINSTALLER_H
