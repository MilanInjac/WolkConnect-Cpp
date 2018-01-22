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

#include "Wolk.h"
#include "BasicConfigLoader.h"
#include <qt5/QtWidgets/QApplication>
#include <qt5/QtCore/QObject>
#include "MainWindow.h"
#include "CustomHandler.h"
#include "BasicFirmwareInstaller.h"
#include "BasicUrlFileDownloader.h"

#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <string>

int main(int argc, char** argv, char** envp)
{
	QApplication a(argc, argv);

	const std::string firmwareVersion = "2.0.0";

	MainWindow mainWindow{firmwareVersion};

	qRegisterMetaType<std::string>("std::string");

	std::string key, password, host;

	if(!example::BasicConfigLoader::load("config", key, password, host))
	{
		return -1;
	}

	std::cout << "key: " << key << ", password: " << password << ", host: " << host << std::endl;

	std::string message;
	example::BasicConfigLoader::loadMessage("config", message);

	wolkabout::Device device(key, password, {"MSG", "STATE"});

	auto customHandler = std::make_shared<example::CustomHandler>();
	QObject::connect(customHandler.get(), &example::CustomHandler::message, &mainWindow, &MainWindow::setMessage);
	QObject::connect(&mainWindow, &MainWindow::messageSet, customHandler.get(), &example::CustomHandler::messageSetSuccess);

	QObject::connect(customHandler.get(), &example::CustomHandler::toggle, &mainWindow, &MainWindow::setToggle);
	QObject::connect(&mainWindow, &MainWindow::toggleSet, customHandler.get(), &example::CustomHandler::toggleSetSuccess);

	auto installer = std::make_shared<example::BasicFirmwareInstaller>(argc, argv, envp);
	auto urlDownloader = std::make_shared<example::BasicUrlFileDownloader>();

    std::unique_ptr<wolkabout::Wolk> wolk =
      wolkabout::Wolk::newBuilder(device)
		.actuationHandler(customHandler)
		.actuatorStatusProvider(customHandler)
		.host(host)
		.withFirmwareUpdate(firmwareVersion, installer, ".", 100 * 1024 * 1024, urlDownloader)
        .build();

	QObject::connect(customHandler.get(), &example::CustomHandler::statusUpdated, [&](std::string ref, wolkabout::ActuatorStatus status){
		wolk->publishActuatorStatus(ref);
	});

	wolk->connect();

	customHandler->handleActuation("MSG", message);
	customHandler->handleActuation("STATE", "true");

    wolk->addSensorReading("TEMPERATURE_REF", 23.4);
    wolk->addSensorReading("BOOL_SENSOR_REF", true);

    wolk->addAlarm("ALARM_REF", "ALARM_MESSAGE_FROM_CONNECTOR");

	mainWindow.show();

	return a.exec();
}
