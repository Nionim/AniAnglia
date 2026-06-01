#!/usr/bin/env python3
import os
from pathlib import Path
from pbxproj import XcodeProject
from pbxproj.pbxextensions.ProjectFiles import TreeType

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
    project_dir = Path(PROJECT_PATH).parent.parent

    root_group_name = Path(SOURCE_ROOT).name
    groups = proj.get_groups_by_name(root_group_name)
    if groups:
        root = groups[0]
        if getattr(root, 'sourceTree', None) != TreeType.SOURCE_ROOT:
            root.sourceTree = TreeType.SOURCE_ROOT
    else:
        root = proj.add_group(
            name=root_group_name,
            path=SOURCE_ROOT.rstrip('/'),
            source_tree=TreeType.SOURCE_ROOT
        )

    source_dir = project_dir / SOURCE_ROOT
    if not source_dir.exists():
        print(f"Cannot find source directory: {source_dir}")
        return

    for ref in proj.objects.get_objects_in_section('PBXFileReference'):
        path = getattr(ref, 'path', None)
        if not path: continue
        if path.endswith("Info.plist") and ("xcframework" in path or Path(path).parent != Path("")):
            if path == "Info.plist": continue
            proj.remove_files_by_path(path, tree=getattr(ref, 'sourceTree', TreeType.SOURCE_ROOT))

    for ref in proj.objects.get_objects_in_section('PBXFileReference'):
        path = getattr(ref, 'path', None)
        if not path: continue
        full = project_dir / path
        if not full.exists() or Path(path).suffix not in ALL_EXTENSIONS:
            proj.remove_files_by_path(path, tree=getattr(ref, 'sourceTree', TreeType.SOURCE_ROOT))
            print(f"Removed: {path}")

    for file_path in source_dir.rglob('*'):
        if not file_path.is_file(): continue
        if any(part in IGNORE_DIRS for part in file_path.parts): continue
        if file_path.suffix not in ALL_EXTENSIONS: continue

        if file_path.name == "Info.plist" and "xcframework" in str(file_path.parents): continue
        rel = str(file_path.relative_to(source_dir)).replace('\\', '/')
        existing = proj.get_files_by_path(rel, tree=TreeType.SOURCE_ROOT)
        if existing: continue

        parent = root
        dirname = os.path.dirname(rel)
        if dirname:
            for comp in dirname.split('/'):
                if comp: parent = proj.get_or_create_group(comp, parent=parent)

        proj.add_file(rel, parent=parent, tree=TreeType.SOURCE_ROOT)
        print(f"Added: {rel}")

    proj.save()

if __name__ == "__main__":
    sync()