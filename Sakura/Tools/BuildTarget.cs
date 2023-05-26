namespace Sakura.BuildTools {
    public class BuildTarget {
        private string name;
        private string sourceFolder;

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
    }
}