using System.Collections.Generic;
using System;
using System.IO;

namespace Sakura.BuildTools {
    public class BuildTarget {
        private string name;
        private string sourceFolder;
        private List<string> includeDirectories = new List<string>();
        private List<string> libraryDirectories = new List<string>();
        private List<string> linkLibraries = new List<string>();

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
            this.includeDirectories.Add(Path.GetFullPath(directory));
        }

        public void AddLibraryDirectory(string directory) {
            this.libraryDirectories.Add(Path.GetFullPath(directory));
        }

        public void LinkLibrary(string library) {
            this.linkLibraries.Add(library);
        }

        public List<string> GetIncludeDirectories() {
            return this.includeDirectories;
        }

        public List<string> GetLibraryDirectories() {
            return this.libraryDirectories;
        }

        public List<string> GetLinkedLibraries() {
            return this.linkLibraries;
        }
    }
}