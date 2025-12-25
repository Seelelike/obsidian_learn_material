```
#!/bin/bash

SRC_DIR="./"

OUT_DIR="${SRC_DIR}/txt_out"

GLOBAL_DEDUP=false

mkdir -p "${OUT_DIR}"

find "${SRC_DIR}" -type f -name "*.srt" | sort | while read -r srt_path; do
	rel_path="${srt_path#$src_dir/}" 
#${srt_path#$SRC_DIR/}：# 删除变量左侧最短匹配模式 "$SRC_DIR/"，获取相对路径。
	out_name="${rel_path//\//__}"
#${rel_path//\//__}：// 表示全局替换，替换所有 / 为 __。
	outpath="$OUT_DIR/${out_name%.srt}.txt"
#${out_name%.srt}：% 删除变量右侧最短匹配 ".srt"，去除扩展名。
	{
		sed '/^[0-9]\+$/d; /^[[:blank:]]*$/d; /^\d\{2\}:\d\{2\}:\d\{2\},[0-9]\{3\}[[:space:]]*-->[[:space:]]*\d\{2\}:\d\{2\}:\d\{2\},[0-9]\{3\}/d' "$srt_path"
	} | if $GLOBAL_DEDUP; then
		sort -u
	else
		uniq
#使用 uniq 命令删除重复的行
	fi > "$out_path"

	echo "OK: $rel_path -> ${out_path#$SRC_DIR/}"
done

echo "Done. Output folder: $(realpath "$OUT_DIR")"
```

```
#!/bin/bash
# 用法: ./download_subtitles_txt.sh "https://www.youtube.com/playlist?list=XXXXX"
# 或单个视频: ./download_subtitles_txt.sh "https://www.youtube.com/watch?v=XXXXX"
if [ -z "$1" ]; then
    echo "错误: 请提供 YouTube URL 作为参数"
    echo "用法: $0 \"YouTube URL\""
    exit 1
fi

URL="$1"

# 你可以修改这里的目标语言，多个用逗号分隔
# all 表示下载所有可用语言
SUB_LANGS="en"   # 默认下载中文和英文字幕

# 输出目录
OUTPUT_DIR="subs"

COOKIES_FILE="E:/Download/www.youtube.com_cookies.txt"


echo "开始下载字幕并转换为纯文本 (.txt) ..."
echo "目标 URL: $URL"
echo "字幕语言: $SUB_LANGS"
echo "输出目录: $OUTPUT_DIR"
echo ""

yt-dlp \
  --cookies "$COOKIES_FILE" \
  --skip-download \
  --write-subs \
  --write-auto-subs \
  --sub-langs "$SUB_LANGS" \
  --convert-subs srt \
  -o "$OUTPUT_DIR/%(playlist_title)s/%(playlist_index)03d - %(title)s.%(ext)s" \
  "$URL"

echo ""
echo "开始清洗字幕 -> 纯文本 .txt（删除序号/时间戳/空行）..."

find "$OUTPUT_DIR" -type f \( -name "*.srt" -o -name "*.vtt" \) -print0 |
while IFS= read -r -d '' f; do
  out="${f%.*}.txt"
  
    case "$f" in
    *.srt)
        sed -E 's/\r$//' "$f" | sed -E \
        -e 's/\\h/ /g' \
        -e 's/[[:space:]]+$//' \
        -e '/^[0-9]+$/d' \
        -e '/^[[:space:]]*$/d' \
        -e '/^[0-9]{2}:[0-9]{2}:[0-9]{2}[,.][0-9]{3}[[:space:]]*-->[[:space:]]*[0-9]{2}:[0-9]{2}:[0-9]{2}[,.][0-9]{3}/d' \
        | uniq > "$out"
        ;;
    *.vtt)
        sed -E 's/\r$//' "$f" | sed -E \
        -e 's/\\h/ /g' \
        -e 's/[[:space:]]+$//' \
        -e '/^WEBVTT/d' \
        -e '/^Kind:/d' \
        -e '/^Language:/d' \
        -e '/^[0-9]+$/d' \
        -e '/^[[:space:]]*$/d' \
        -e '/^[0-9]{2}:[0-9]{2}(:[0-9]{2})?\.[0-9]{3}[[:space:]]*-->[[:space:]]*[0-9]{2}:[0-9]{2}(:[0-9]{2})?\.[0-9]{3}.*/d' \
        | uniq > "$out"
        ;;
    esac

  # 如果输出非空，则删掉原字幕文件
  if [[ -s "$out" ]]; then
    rm -f "$f"
    echo "OK: $(basename "$out")"
  else
    echo "WARN: 输出为空，保留原文件: $f"
    rm -f "$out"
  fi
done

echo ""
echo "完成！纯文本字幕已保存到 $OUTPUT_DIR 目录下（扩展名为 .txt）"
```

问题1：yt-dlp下载文件，需要通过bot验证，要使用cookies，edge浏览器的cookies加密方式加强了，yt-dlp 在 Windows 上用 `--cookies-from-browser edge` 无法用 DPAPI 解密。
解决：手动导出cookies文件，并作为参数传给yt-dlp。

问题2：anaconda的python环境安装了yt-dlp，但window终端找不到该命令。
yt-dlp本质是一个Python包+命令行脚本。
安装时，pip 会做两件事：
1. 把 `yt_dlp` 模块装进某个 Python 环境
2. 在 该 Python 环境的 `Scripts/` 目录下生成一个 `yt-dlp.exe`
`C:\Users\你\anaconda3\Scripts\yt-dlp.exe`
[[anaconda Prompt路径添加|anaconda prompt]]启动时会激活base conda环境，并把`C:\Users\你\anaconda3\
C:\Users\你\anaconda3\Scripts\`添加进PATH。

解决：在其他终端激活环境。在 **Anaconda Prompt** 里运行：[[anaconda init|conda init]]，然后在其他终端激活环境。

