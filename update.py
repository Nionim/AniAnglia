#!/usr/bin/env python3

# СКРИПТ НАНЕЙРОСЛОПЛЕН
# СКРИПТ НАНЕЙРОСЛОПЛЕН
# СКРИПТ НАНЕЙРОСЛОПЛЕН

import os
from pathlib import Path
from pbxproj import XcodeProject
from pbxproj.pbxextensions.ProjectFiles import TreeType
from pbxproj import PBXGenericObject

PROJECT_PATH = "AniSaturn.xcodeproj/project.pbxproj"
SOURCE_ROOT = "AniSaturn/"

SOURCE_EXTENSIONS = {'.m', '.mm', '.h', '.c', '.cpp', '.swift', '.strings', '.xib', '.storyboard'}
RESOURCE_EXTENSIONS = {'.strings', '.xib', '.storyboard', '.xcassets', '.plist', '.png', '.jpg'}
ALL_EXTENSIONS = SOURCE_EXTENSIONS | RESOURCE_EXTENSIONS
IGNORE_DIRS = {'.git', 'build', 'DerivedData', 'Pods', 'node_modules', '__pycache__', '.idea', '.vscode', '.zed'}

def is_inside_xcframework(path: str) -> bool:
    parts = Path(path).parts
    return any('.xcframework' in part for part in parts)

def fix_localization(proj, source_dir, root_group):
    lproj_files = {}
    for lproj_dir in source_dir.glob("*.lproj"):
        strings_file = lproj_dir / "Localizable.strings"
        if strings_file.is_file():
            lang = lproj_dir.name.split('.')[0]
            rel_path = strings_file.relative_to(source_dir)
            lproj_files[lang] = str(rel_path).replace('\\', '/')

    if not lproj_files:
        print("No localization files found, skipping fix.")
        return

    variant_group = None
    for obj in proj.objects.get_objects_in_section('PBXVariantGroup'):
        if getattr(obj, 'name', None) == 'Localizable.strings':
            variant_group = obj
            break

    if variant_group:
        print(f"Removing old variant group {variant_group.get_id()}")
        for child_id in getattr(variant_group, 'children', []):
            if child_id in proj.objects: proj.remove_file_by_id(child_id)
        del proj.objects[variant_group.get_id()]

    file_refs = []
    for lang, rel_path in lproj_files.items():
        file_ref = PBXGenericObject().parse({
            'isa': 'PBXFileReference',
            '_id': PBXGenericObject._generate_id(),
            'lastKnownFileType': 'text.plist.strings',
            'name': 'Localizable.strings',
            'path': rel_path,
            'sourceTree': TreeType.SOURCE_ROOT
        })
        proj.objects[file_ref.get_id()] = file_ref
        file_refs.append(file_ref.get_id())
        print(f"Created file reference for {lang}: {rel_path}")

    new_variant = PBXGenericObject().parse({
        'isa': 'PBXVariantGroup',
        '_id': PBXGenericObject._generate_id(),
        'name': 'Localizable.strings',
        'children': file_refs,
        'sourceTree': '<group>'
    })
    proj.objects[new_variant.get_id()] = new_variant

    parent_group = root_group
    for obj in proj.objects.get_objects_in_section('PBXGroup'):
        if hasattr(obj, 'children'):
            for child_id in obj.children:
                if child_id not in proj.objects: continue
                child = proj.objects[child_id]
                if child and getattr(child, 'path', '').endswith('.lproj'):
                    parent_group = obj
                    break
        if parent_group != root_group: break

    parent_group.children.append(new_variant.get_id())
    print(f"Added variant group to parent group {parent_group.get_id()}")

    for target in proj.objects.get_targets():
        for build_phase_id in target.buildPhases:
            build_phase = proj.objects[build_phase_id]
            if build_phase.isa == 'PBXResourcesBuildPhase':
                for build_file_id in list(build_phase.files):
                    if build_file_id not in proj.objects: continue
                    build_file = proj.objects[build_file_id]
                    if build_file and hasattr(build_file, 'fileRef'):
                        if build_file.fileRef not in proj.objects: continue
                        file_ref = proj.objects[build_file.fileRef]
                        if file_ref and getattr(file_ref, 'path', '').endswith('Localizable.strings'):
                            build_phase.files.remove(build_file_id)
                            del proj.objects[build_file_id]
                build_file = PBXGenericObject().parse({
                    'isa': 'PBXBuildFile',
                    '_id': PBXGenericObject._generate_id(),
                    'fileRef': new_variant.get_id()
                })
                proj.objects[build_file.get_id()] = build_file
                build_phase.files.append(build_file.get_id())
                print(f"Added build file for variant group to Resources phase of target {target.name}")

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
        if path.endswith('Info.plist') and is_inside_xcframework(path):
            print(f"Removing xcframework Info.plist: {path}")
            proj.remove_files_by_path(path, tree=getattr(ref, 'sourceTree', TreeType.SOURCE_ROOT))

    for ref in proj.objects.get_objects_in_section('PBXFileReference'):
        path = getattr(ref, 'path', None)
        if not path: continue
        if path == "Info.plist": continue
        if path.endswith('.lproj/Localizable.strings'): continue
        full = project_dir / path
        if not full.exists() or Path(path).suffix not in ALL_EXTENSIONS:
            proj.remove_files_by_path(path, tree=getattr(ref, 'sourceTree', TreeType.SOURCE_ROOT))
            print(f"Removed: {path}")

    root_plist_refs = proj.get_files_by_path("Info.plist", tree=TreeType.SOURCE_ROOT)
    for ref in root_plist_refs:
        build_files = proj.get_build_files_for_file(ref.get_id())
        for bf in build_files:
            bf.remove(recursive=True)

    fix_localization(proj, source_dir, root)
    for file_path in source_dir.rglob('*'):
        if not file_path.is_file(): continue
        if any(part in IGNORE_DIRS for part in file_path.parts): continue
        if file_path.suffix not in ALL_EXTENSIONS: continue

        rel_path = file_path.relative_to(source_dir)
        rel = str(rel_path).replace('\\', '/')

        if rel == "Info.plist": continue
        if file_path.name == "Info.plist" and is_inside_xcframework(str(rel_path)): continue
        if file_path.name == "Localizable.strings" and ".lproj" in str(rel_path): continue

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