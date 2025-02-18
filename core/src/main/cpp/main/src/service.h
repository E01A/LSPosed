/*
 * This file is part of LSPosed.
 *
 * LSPosed is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LSPosed is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LSPosed.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2021 - 2022 LSPosed Contributors
 */

//
// Created by loves on 2/7/2021.
//

#ifndef LSPOSED_SERVICE_H
#define LSPOSED_SERVICE_H

#include <jni.h>
#include "context.h"

using namespace std::literals::string_view_literals;

namespace lspd {
    class Service {
        constexpr static jint DEX_TRANSACTION_CODE = 1310096052;
        constexpr static jint BRIDGE_TRANSACTION_CODE = 1598837584;
        constexpr static auto BRIDGE_SERVICE_DESCRIPTOR = "LSPosed"sv;
        constexpr static auto BRIDGE_SERVICE_NAME = "activity"sv;
        constexpr static auto SYSTEM_SERVER_BRIDGE_SERVICE_NAME = "serial"sv;
        constexpr static jint BRIDGE_ACTION_GET_BINDER = 2;
        inline static jint SET_ACTIVITY_CONTROLLER_CODE = -1;

    public:
        inline static Service* instance() {
            return instance_.get();
        }

        inline static std::unique_ptr<Service> ReleaseInstance() {
            return std::move(instance_);
        }

        void InitService(JNIEnv *env);

        void HookBridge(const Context& context, JNIEnv *env);
        ScopedLocalRef<jobject> RequestBinder(JNIEnv *env, jstring nice_name);

        ScopedLocalRef<jobject> RequestSystemServerBinder(JNIEnv *env);

        ScopedLocalRef<jobject> RequestApplicationBinderFromSystemServer(JNIEnv *env, const ScopedLocalRef<jobject> &system_server_binder);

        std::tuple<int, size_t> RequestLSPDex(JNIEnv *env, const ScopedLocalRef<jobject> &binder);

    private:
        inline static std::unique_ptr<Service> instance_ = std::make_unique<Service>();
        bool initialized_ = false;

        Service() = default;

        static jboolean
        call_boolean_method_va_replace(JNIEnv *env, jobject obj, jmethodID methodId, va_list args);

        static jboolean exec_transact_replace(jboolean *res, JNIEnv *env, jobject obj, va_list args);

        JNINativeInterface native_interface_replace_{};
        jmethodID exec_transact_backup_methodID_ = nullptr;

        jboolean (*call_boolean_method_va_backup_)(JNIEnv *env, jobject obj, jmethodID methodId,
                                                  va_list args) = nullptr;

        jclass bridge_service_class_ = nullptr;
        jmethodID exec_transact_replace_methodID_ = nullptr;
        jmethodID replace_activity_controller_methodID_ = nullptr;
        jmethodID replace_shell_command_methodID_ = nullptr;

        jclass binder_class_ = nullptr;
        jmethodID binder_ctor_ = nullptr;

        jclass service_manager_class_ = nullptr;
        jmethodID get_service_method_ = nullptr;

        jmethodID transact_method_ = nullptr;

        jclass parcel_class_ = nullptr;
        jmethodID obtain_method_ = nullptr;
        jmethodID recycleMethod_ = nullptr;
        jmethodID write_interface_token_method_ = nullptr;
        jmethodID write_int_method_ = nullptr;
        jmethodID write_string_method_ = nullptr;
        jmethodID read_exception_method_ = nullptr;
        jmethodID read_strong_binder_method_ = nullptr;
        jmethodID write_strong_binder_method_ = nullptr;
        jmethodID read_file_descriptor_method_ = nullptr;
        jmethodID read_long_method_ = nullptr;
        jmethodID read_string_method_ = nullptr;

        jclass parcel_file_descriptor_class_ = nullptr;
        jmethodID detach_fd_method_ = nullptr;

        jclass deadObjectExceptionClass_ = nullptr;

        friend std::unique_ptr<Service> std::make_unique<Service>();

    };
}


#endif //LSPOSED_SERVICE_H
