// SPDX-FileCopyrightText: Copyright 2018 yuzu Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "core/hle/service/am/am.h"
#include "core/hle/service/am/button_poller.h"
#include "core/hle/service/am/caps_su.h"
#include "core/hle/service/am/event_observer.h"
#include "core/hle/service/am/service/all_system_applet_proxies_service.h"
#include "core/hle/service/am/service/application_proxy_service.h"
#include "core/hle/service/am/window_system.h"
#include "core/hle/service/apm/apm.h"
#include "core/hle/service/apm/apm_interface.h"
#include "core/hle/service/caps/caps_manager.h"
#include "core/hle/service/server_manager.h"

namespace Service::AM {

void LoopProcess(Core::System& system) {
    WindowSystem window_system(system);
    ButtonPoller button_poller(system, window_system);
    EventObserver event_observer(system, window_system);

    auto album_manager = std::make_shared<Capture::AlbumManager>(system);
    auto module = std::make_shared<APM::Module>();
    auto server_manager = std::make_unique<ServerManager>(system);

    server_manager->RegisterNamedService(
        "apm", std::make_shared<APM::APM>(system, module, system.GetAPMController(), "apm"));
    server_manager->RegisterNamedService(
        "appletAE", std::make_shared<IAllSystemAppletProxiesService>(system, window_system));
    server_manager->RegisterNamedService(
        "appletOE", std::make_shared<IApplicationProxyService>(system, window_system));
    ServerManager::RunServer(std::move(server_manager));
    server_manager->RegisterNamedService(
        "caps:su", std::make_shared<Capture::IScreenShotApplicationService>(system, album_manager));
}

} // namespace Service::AM
