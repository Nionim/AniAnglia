#!/usr/bin/env python3
import os
from pathlib import Path
from pbxproj import XcodeProject

PROJECT_PATH = "AniSaturn.xcodeproj/project.pbxproj"
SOURCE_ROOT = "AniSaturn/"

SOURCE_EXTENSIONS = {'.m', '.mm', '.h', '.c', '.cpp', '.swift', '.strings', '.xib', '.storyboard'}
RESOURCE_EXTENSIONS = {'.strings', '.xib', '.storyboard', '.xcassets', '.plist', '.png', '.jpg'}
ALL_EXTENSIONS = SOURCE_EXTENSIONS | RESOURCE_EXTENSIONS
IGNORE_DIRS = {'.git', 'build', 'DerivedData', 'Pods', 'node_modules', '__pycache__', '.idea', '.vscode', '.zed'}

def sync():
    if not os.path.exists(PROJECT_PATH):
        print(f"Project file not found: {PROJECT_PATH}")
        return

    proj = XcodeProject.load(PROJECT_PATH)
    root_group_name = Path(SOURCE_ROOT).name
    root = proj.get_or_create_group(root_group_name)

    project_dir = Path(PROJECT_PATH).parent.parent
    source_dir = project_dir / SOURCE_ROOT
    if not source_dir.exists():
        print(f"Cannot find source directory: {source_dir}")
        return

    # Удаляем отсутствующие файлы
    for ref in proj.objects.get_objects('PBXFileReference'):
        path = ref.get('path')
        if not path:
            continue
        full = project_dir / path
        if not full.exists() or Path(path).suffix not in ALL_EXTENSIONS:
            proj.remove_file_by_path(path)

    # Добавляем новые файлы
    for file_path in source_dir.rglob('*'):
        if not file_path.is_file():
            continue
        if any(p in IGNORE_DIRS for p in file_path.parts):
            continue
        if file_path.suffix not in ALL_EXTENSIONS:
            continue

        rel = str(file_path.relative_to(source_dir)).replace('\\', '/')
        if proj.get_file_by_path(rel):
            continue

        group = root
        for comp in os.path.dirname(rel).split('/'):
            if comp:
                group = group.get_or_create_group(comp)

        fr = proj.add_file(rel, parent=group, tree='SOURCE_ROOT')
        if fr:
            if file_path.suffix in SOURCE_EXTENSIONS:
                proj.add_file_to_build_phase(fr, phase='sources')
            elif file_path.suffix in RESOURCE_EXTENSIONS:
                proj.add_file_to_build_phase(fr, phase='resources')

    proj.save()
    print("Synchronization complete.")

if __name__ == "__main__":
    sync()