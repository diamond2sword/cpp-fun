#!/bin/bash

main () {
	local cpp_path_list
	cpp_path_list=$(find . | all_suffixed cpp | all_not 'main\.cpp')
	for i in $(seq 1 "$(echo "$cpp_path_list" | wc -l)"); do
		local cpp_path
		cpp_path=$(echo "$cpp_path_list" | get_line "$i")
		local dir_part
		dir_part=$(dirname "$cpp_path")
		local base_part
		base_part=$(basename "$cpp_path")
		name_part=$(echo "$base_part" | get_file_name)
		mkdir -p "$dir_part/$name_part"
		cp "$cpp_path" "$dir_part/$name_part/main.cpp" &> /dev/null
	done
}

all_not () {
	sed "/$1/d"
}

get_file_name () {
	sed -E "s/^([^\.]*)(\.[^\.]*)*$/\1/"
}

get_line () {
	sed -n "$1p"
}

all_suffixed () {
	sed "/$1$/!d"
}

main "$@"
