/*
 * SPDX-FileCopyrightText: Copyright (c) 2022-2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "tensorrt_llm/executor/executor.h"
#include "tensorrt_llm/runtime/common.h"
#include <filesystem>
#include <optional>

namespace fs = std::filesystem;

namespace tensorrt_llm::batch_manager
{

using runtime::SizeType;

struct PeftCacheManagerConfig
{

    static float constexpr kDefaultDeviceCachePercent = 0.05;
    static size_t constexpr kDefaultHostCacheSize = 1024 * 1024 * 1024;

    explicit PeftCacheManagerConfig(SizeType numHostModuleLayer = 0, SizeType numDeviceModuleLayer = 0,
        SizeType optimalAdapterSize = 8, SizeType maxAdapterSize = 64, SizeType numPutWorkers = 1,
        SizeType numEnsureWorkers = 1, SizeType numCopyStreams = 1, SizeType maxPagesPerBlockHost = 24,
        SizeType maxPagesPerBlockDevice = 8, std::optional<float> deviceCachePercent = std::nullopt,
        std::optional<size_t> hostCacheSize = std::nullopt)
        : numHostModuleLayer(numHostModuleLayer)
        , numDeviceModuleLayer(numDeviceModuleLayer)
        , optimalAdapterSize(optimalAdapterSize)
        , maxAdapterSize(maxAdapterSize)
        , numPutWorkers(numPutWorkers)
        , numEnsureWorkers(numEnsureWorkers)
        , numCopyStreams(numCopyStreams)
        , maxPagesPerBlockHost(maxPagesPerBlockHost)
        , maxPagesPerBlockDevice(maxPagesPerBlockDevice)
        , deviceCachePercent(deviceCachePercent)
        , hostCacheSize(hostCacheSize)
    {
    }

    PeftCacheManagerConfig(executor::PeftCacheConfig cfg)
        : numHostModuleLayer(cfg.getNumHostModuleLayer())
        , numDeviceModuleLayer(cfg.getNumDeviceModuleLayer())
        , optimalAdapterSize(cfg.getOptimalAdapterSize())
        , maxAdapterSize(cfg.getMaxAdapterSize())
        , numPutWorkers(cfg.getNumPutWorkers())
        , numEnsureWorkers(cfg.getNumEnsureWorkers())
        , numCopyStreams(cfg.getNumCopyStreams())
        , maxPagesPerBlockHost(cfg.getMaxPagesPerBlockHost())
        , maxPagesPerBlockDevice(cfg.getMaxPagesPerBlockDevice())
        , deviceCachePercent(cfg.getDeviceCachePercent())
        , hostCacheSize(cfg.getHostCacheSize())
    {
    }

    // number of max sized 1-layer 1-module adapterSize=1 sets of weights that can be stored in host cache
    SizeType numHostModuleLayer;
    // number of max sized 1-layer 1-module sets of weights that can be stored in host cache
    SizeType numDeviceModuleLayer;
    // optimal adapter size used to set page width
    SizeType optimalAdapterSize;
    // max supported adapter size. Used to compute minimum
    SizeType maxAdapterSize;
    // number of worker threads used to put weights into host cache
    SizeType numPutWorkers;
    // number of worker threads used to copy weights from host to device
    SizeType numEnsureWorkers;
    // number of streams used to copy weights from host to device
    SizeType numCopyStreams;
    // Number of cache pages per allocation block (host)
    SizeType maxPagesPerBlockHost;
    // Number of cache pages per allocation block (device)
    SizeType maxPagesPerBlockDevice;
    // percent of memory after engine load to use for cache
    std::optional<float> deviceCachePercent;
    // size in bytes to use for host cache
    std::optional<size_t> hostCacheSize;
};
} // namespace tensorrt_llm::batch_manager
