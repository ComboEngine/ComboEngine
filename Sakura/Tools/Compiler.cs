using Sakura.BuildTools;
using System;
using System.IO;
using System.Diagnostics;

namespace Sakura.Tools {
    public class Compiler {
        public static void CompileElement(String targetPath,BuildTarget target) {
            if(target.GetBuildType() == BuildType.Project) {

            } else {
                String path = Path.GetDirectoryName(targetPath) + "\\" + target.GetName() + ".vcxproj";
                Directory.CreateDirectory(target.GetName() + "_Dist");
                cmdExec("C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\Common7\\IDE\\devenv.exe", Path.GetFullPath(path) + " /Build /ProjectConfig Release|x64");
                File.Copy(Path.GetDirectoryName(targetPath) + "\\Binary\\" + target.GetName() + (target.GetBuildType() == BuildType.Executable ? ".exe" : ".dll"),target.GetName() + "_Dist\\" + target.GetName() + (target.GetBuildType() == BuildType.Executable ? ".exe" : ".dll"));
            }
        }
        public static void cmdExec(string file,string command) {
            var proc = new Process 
            {
                StartInfo = new ProcessStartInfo
                {
                    FileName = file,
                    Arguments = command,
                    UseShellExecute = false,
                    RedirectStandardOutput = true,
                    CreateNoWindow = true
                }
            };
            proc.Start();
            while (!proc.StandardOutput.EndOfStream)
            {
                string line = proc.StandardOutput.ReadLine();
                Console.WriteLine(line);
            }
        }
    }
}