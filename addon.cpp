#include <napi.h>
#include <windows.h>
#include <lm.h>
#pragma comment(lib, "netapi32.lib")

Napi::String GetUserPrivilege(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Expected a string as the first argument").ThrowAsJavaScriptException();
        return Napi::String::New(env, "Invalid input");
    }

    std::string username = info[0].As<Napi::String>().Utf8Value();
    std::wstring wUsername(username.begin(), username.end());

    LPUSER_INFO_3 userInfo = nullptr;
    NET_API_STATUS status = NetUserGetInfo(nullptr, wUsername.c_str(), 3, (LPBYTE*)&userInfo);

    if (status != NERR_Success) {
        return Napi::String::New(env, "User not found");
    }

    std::string privilege;
    switch (userInfo->usri3_priv) {
        case USER_PRIV_GUEST:
            privilege = "Гость";
            break;
        case USER_PRIV_USER:
            privilege = "Пользователь";
            break;
        case USER_PRIV_ADMIN:
            privilege = "Администратор";
            break;
        default:
            privilege = "Неизвестно";
    }

    NetApiBufferFree(userInfo);
    return Napi::String::New(env, privilege);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "getUserPrivilege"), Napi::Function::New(env, GetUserPrivilege));
    return exports;
}

NODE_API_MODULE(addon, Init)