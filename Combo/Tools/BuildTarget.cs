using System.Collections.Generic;
using System;
using System.IO;

namespace Combo.BuildTools {
    public class BuildTarget {
        private string name;
        private string sourceFolder;
        public List<string> includeDirectories = new List<string>();
        public List<string> libraryDirectories = new List<string>();
        public List<string> linkLibraries = new List<string>();
        public List<string> modules = new List<string>();
        public List<string> compilerDefinitions = new List<string>();
        private BuildType buildType = BuildType.Executable;

        public string Localization;

        public string GetName() {
            return this.name;
        }

        public void SetName(string name) {
            this.name = name;
        }

        public void AddSourceFolder(string sourceFolder) {
            this.sourceFolder = sourceFolder;
        }

        public string GetSourceFolder() {
            return this.sourceFolder;
        }

        public void AddIncludeDirectory(string directory) {
            this.includeDirectories.Add(directory);
        }

        public void AddDefinition(string def) {
            this.compilerDefinitions.Add(def);
        }


        public void AddLibraryDirectory(string directory) {
            this.libraryDirectories.Add(directory);
        }

        public void LinkLibrary(string library) {
            this.linkLibraries.Add(library);
        }

        public List<string> GetIncludeDirectories() {
            return this.includeDirectories;
        }

        public List<string> GetDefinitions() {
            return this.compilerDefinitions;
        }

        public List<string> GetLibraryDirectories() {
            return this.libraryDirectories;
        }

        public List<string> GetLinkedLibraries() {
            return this.linkLibraries;
        }

        public void AddModule(string module) {
            this.modules.Add(module);
        }

        public List<string> GetModules() {
            return this.modules;
        }

        public void SetBuildType(BuildType type) {
            this.buildType = type;
        }

        public BuildType GetBuildType() {
            return this.buildType;
        }

    }
}