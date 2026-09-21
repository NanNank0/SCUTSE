from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
import requests
import uvicorn
import re
import time

app = FastAPI()
app.add_middleware(CORSMiddleware, allow_origins=["*"])

DEEPSEEK_KEY = "sk-345fbdea8e5946b9a9077365316b7bd6"

"""
这个沙比OSC会有很奇怪的东西输出出来
而且dsv4的输出会先输出思考内容，即reasoning_content，再输出content
"""
def clean_text(text):
    if not text:
        return ""
    text = text.replace('\n', ' ').replace('\r', ' ')
    text = re.sub(r'\s+', ' ', text)
    text = re.sub(r'（\s+）', '（）', text)
    return text.strip()
@app.get("/query")
async def query(title: str = "", options: str = "", type: str = ""):
    title = clean_text(title)
    options = clean_text(options)
    
    print(f"\n📖 题目: {title[:50]}")
    print(f"📌 题型: {type}")
    print(f"📋 选项: {options[:80]}")
    
    # 判断题特殊处理
    if type == "judgement":
        prompt = f"题目：{title}\n这是判断题，请回答'正确'或'错误'，只输出这两个字。"
    elif type == "multiple":
        prompt = f"题目：{title}\n选项：{options}\n这是多选题，输出所有正确答案的字母，用#连接。"
    else:
        prompt = f"题目：{title}\n选项：{options}\n只输出答案字母。"
    
    try:
        resp = requests.post(
            "https://api.deepseek.com/v1/chat/completions",
            headers={
                "Authorization": f"Bearer {DEEPSEEK_KEY}",
                "Content-Type": "application/json"
            },
            json={
                "model": "deepseek-v4-flash",
                "messages": [{"role": "user", "content": prompt}],
                "temperature": 0,
                "max_tokens": 500
            },
            timeout=30
        )
        
        if resp.status_code != 200:
            print(f"❌ HTTP {resp.status_code}")
            return {"code": 0, "msg": f"HTTP {resp.status_code}"}
        
        data = resp.json()
        message = data["choices"][0]["message"]
        answer = message.get("content", "").strip()
        reasoning = message.get("reasoning_content", "")
        
        print(f"📝 content: '{answer}'")
        
        # 判断题
        if type == "judgement":
            if "正确" in answer:
                answer = "正确"
            elif "错误" in answer:
                answer = "错误"
            else:
                # 从 reasoning 提取
                if "正确" in reasoning:
                    answer = "正确"
                elif "错误" in reasoning:
                    answer = "错误"
                else:
                    answer = "正确"  # 默认
            print(f"✅ 判断题答案: {answer}")
            return {"code": 1, "question": title, "answer": answer}
        
        # 多选题
        if type == "multiple":
            letters = re.findall(r'[ABCD]', answer)
            if not letters:
                letters = re.findall(r'[ABCD]', reasoning)
            if letters:
                seen = set()
                unique = []
                for ch in letters:
                    if ch not in seen:
                        seen.add(ch)
                        unique.append(ch)
                answer = '#'.join(unique)
                print(f"✅ 多选题答案: {answer}")
                return {"code": 1, "question": title, "answer": answer}
        
        # 单选题
        match = re.search(r'[ABCD]', answer)
        if not match:
            match = re.search(r'[ABCD]', reasoning)
        if match:
            answer = match.group()
            print(f"✅ 单选题答案: {answer}")
            return {"code": 1, "question": title, "answer": answer}
        
    except Exception as e:
        print(f"❌ 错误: {e}")
        return {"code": 0, "msg": str(e)}

if __name__ == "__main__":
    print("🚀 OCS DeepSeek 中间件 (支持判断题)")
    uvicorn.run(app, host="127.0.0.1", port=8080)