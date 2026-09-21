========================================
   OCS DeepSeek 中间件 - 使用说明
========================================

【启动步骤】
1. 双击运行 start.bat
2. 看到 "Uvicorn running on http://127.0.0.1:8080" 表示启动成功
3. 保持窗口不要关闭

【OCS 配置】
在 OCS 悬浮窗 → 通用 → 全局设置 → 题库配置 中填入：

[
    {
        "name": "DeepSeek本地",
        "url": "http://127.0.0.1:8080/query",
        "method": "get",
        "type": "fetch",
        "contentType": "json",
        "data": {
            "title": "${title}",
            "options": "${options}",
            "type": "${type}"
        },
        "handler": "return (res) => { if(res && res.code === 1 && res.answer) { return [res.question, res.answer]; } return undefined; }"
    }
]

【测试】
浏览器访问 http://127.0.0.1:8080/
显示 {"status":"ok"} 表示中间件运行正常

【注意事项】
- 运行期间不要关闭命令行窗口
- 确保 API Key 已填入 server.py
- 使用完后按 Ctrl+C 停止服务