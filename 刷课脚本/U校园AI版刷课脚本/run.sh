#!/usr/bin/env bash
set -euo pipefail

# Run UnipusHelperPro with a Java 25+ runtime when available.

JAR_NAME="UnipusHelperPro-1.0.3.jar"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
JAR_PATH="${SCRIPT_DIR}/${JAR_NAME}"
BACKGROUND=1

if [[ "${1:-}" == "--foreground" ]]; then
  BACKGROUND=0
  shift
fi

if [[ ! -f "${JAR_PATH}" ]]; then
  echo "未找到文件: ${JAR_PATH}" >&2
  exit 1
fi

extract_major() {
  local version_string="$1"
  if [[ "${version_string}" =~ ^1\.([0-9]+) ]]; then
    echo "${BASH_REMATCH[1]}"
  elif [[ "${version_string}" =~ ^([0-9]+) ]]; then
    echo "${BASH_REMATCH[1]}"
  else
    echo ""
  fi
}

java_major_version() {
  local java_bin="$1"
  local raw version
  raw=$("${java_bin}" -version 2>&1 | head -n 1 || true)
  version=$(echo "${raw}" | sed -n 's/.*version "\(.*\)".*/\1/p')
  extract_major "${version}"
}

list_all_javas() {
  local seen_paths=()
  local -A seen
  while IFS= read -r path; do
    [[ -z "${path}" ]] && continue
    if [[ -z "${seen[${path}]:-}" ]]; then
      seen_paths+=("${path}")
      seen["${path}"]=1
    fi
  done < <(command -v -a java 2>/dev/null || true)
  if command -v which >/dev/null 2>&1; then
    while IFS= read -r path; do
      [[ -z "${path}" ]] && continue
      if [[ -z "${seen[${path}]:-}" ]]; then
        seen_paths+=("${path}")
        seen["${path}"]=1
      fi
    done < <(which -a java 2>/dev/null || true)
  fi
  printf "%s\n" "${seen_paths[@]}"
}

prompt_for_java() {
  local -a candidates=("$@")
  local choice
  while true; do
    echo "请选择要使用的 Java："
    local idx=1
    for bin in "${candidates[@]}"; do
      local major
      major=$(java_major_version "${bin}")
      printf "%d) %s (version: %s)\n" "${idx}" "${bin}" "${major:-unknown}"
      idx=$((idx + 1))
    done
    printf "%d) 自定义 Java 路径\n" "${idx}"
    read -rp "输入序号: " choice
    if [[ "${choice}" =~ ^[0-9]+$ ]] && ((choice >= 1 && choice <= idx)); then
      if (( choice == idx )); then
        local custom
        read -rp "请输入 Java 可执行文件的完整路径: " custom
        if [[ -x "${custom}" ]]; then
          echo "${custom}"
          return
        else
          echo "路径无效或不可执行: ${custom}" >&2
        fi
      else
        echo "${candidates[choice-1]}"
        return
      fi
    else
      echo "输入无效，请重试。" >&2
    fi
  done
}

run_jar() {
  local java_bin="$1"
  if [[ "${BACKGROUND}" -eq 1 ]]; then
    nohup "${java_bin}" -jar "${JAR_PATH}" >/dev/null 2>&1 &
    echo "已使用 ${java_bin} 在后台启动，PID: $!"
  else
    "${java_bin}" -jar "${JAR_PATH}"
  fi
}

DEFAULT_JAVA="$(command -v java || true)"
SELECTED_JAVA=""

if [[ -n "${DEFAULT_JAVA}" ]]; then
  default_major=$(java_major_version "${DEFAULT_JAVA}")
  if [[ "${default_major:-0}" -ge 25 ]]; then
    SELECTED_JAVA="${DEFAULT_JAVA}"
    echo "默认 Java 满足要求 (version ${default_major})."
  fi
fi

if [[ -z "${SELECTED_JAVA}" ]]; then
  mapfile -t ALL_JAVAS < <(list_all_javas)
  for bin in "${ALL_JAVAS[@]}"; do
    [[ "${bin}" == "${DEFAULT_JAVA}" ]] && continue
    major=$(java_major_version "${bin}")
    if [[ "${major:-0}" -ge 25 ]]; then
      SELECTED_JAVA="${bin}"
      echo "在 PATH 中找到符合要求的 Java: ${bin} (version ${major})."
      break
    fi
  done
fi

if [[ -z "${SELECTED_JAVA}" ]]; then
  echo "未在 PATH 中找到 25 及以上版本的 Java。"
  mapfile -t ALL_JAVAS < <(list_all_javas)
  if [[ ${#ALL_JAVAS[@]} -eq 0 ]]; then
    echo "当前 PATH 中未发现任何 Java，可输入自定义路径。"
  fi
  SELECTED_JAVA=$(prompt_for_java "${ALL_JAVAS[@]}")
  selected_major=$(java_major_version "${SELECTED_JAVA}")
  if [[ -n "${selected_major}" && "${selected_major}" -lt 25 ]]; then
    echo "提示: 所选 Java 版本为 ${selected_major}，低于 25。"
  fi
fi

if [[ -z "${SELECTED_JAVA}" ]]; then
  echo "未选择 Java，退出。" >&2
  exit 1
fi

run_jar "${SELECTED_JAVA}"
