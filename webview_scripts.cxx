Core19::main->AddScriptToExecuteOnDocumentCreated(js_inject_embed().c_str(), nullptr);

Core::main->AddScriptToExecuteOnDocumentCreated(js_inject().c_str(), nullptr);

// Core19::side->AddScriptToExecuteOnDocumentCreated(
//     js_inject().c_str(),
//     Callback<
//         ICoreWebView2AddScriptToExecuteOnDocumentCreatedCompletedHandler>(
//         [](HRESULT hr, PCWSTR id) -> HRESULT
//         {
//             Core19::side->ExecuteScript(
//                 js_inject().c_str(),
//                 Callback<
//                     ICoreWebView2ExecuteScriptCompletedHandler>(
//                     [](HRESULT hr, PCWSTR id) -> HRESULT
//                     {
//                         println("SUCCESS SCRIPT INJECT");
//                         return S_OK;
//                     })
//                     .Get());

//             return S_OK;
//         })
//         .Get());

// Core19::side->ExecuteScript(js_inject().c_str(), nullptr);

// Core19::side->AddScriptToExecuteOnDocumentCreated(
//     js_inject().c_str(),
//     Callback<
//         ICoreWebView2AddScriptToExecuteOnDocumentCreatedCompletedHandler>(
//         [](HRESULT hr, PCWSTR id) -> HRESULT
//         {
//             Core19::side->ExecuteScript(js_inject().c_str(),
//                                         nullptr);

//             return S_OK;
//         })
//         .Get());
