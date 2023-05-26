using System.Collections.Generic;
using System;

namespace Sakura.BuildTools {
    public class BuildTarget {
        private string name;
        private string sourceFolder;
        private List<string> includeDirectories = new List<string>();

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

        public List<string> GetIncludeDirectories() {
            return this.includeDirectories;
        }
    }
}