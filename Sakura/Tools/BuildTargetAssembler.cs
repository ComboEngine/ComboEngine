using System;
using System.Collections.Generic;
using System.CodeDom.Compiler;
using System.IO;
using System.Linq;
using System.Reflection;
using Newtonsoft.Json;

namespace Sakura.Tools {
    public class BuildTargetAssembler {
        public static Assembly buildAssembly(string path) {
            Dictionary<string, string> providerOptions = new Dictionary<string, string>();
            providerOptions.Add("CompilerVersion", "v4.0");
            CodeDomProvider provider = new Microsoft.CSharp.CSharpCodeProvider(providerOptions);

            CompilerParameters cp = new CompilerParameters();
            cp.GenerateExecutable = false;
            cp.WarningLevel = 4;
            cp.TreatWarningsAsErrors = false;
            cp.GenerateInMemory = true;
            cp.ReferencedAssemblies.Add(typeof(ArgumentParser).Assembly.Location);
            cp.ReferencedAssemblies.Add(typeof(JsonConvert).Assembly.Location);
            cp.IncludeDebugInformation = false;

            CompilerResults cr = provider.CompileAssemblyFromFile(cp, path);

            bool hasError = false;
            foreach (CompilerError ce in cr.Errors)
            {
                if (ce.IsWarning)
                {
                    Console.WriteLine(string.Format("{0} at {1}: {2}", ce.FileName, ce.Line, ce.ErrorText));
                }
                else
                {
                    Console.WriteLine(string.Format("{0} at line {1}: {2}", ce.FileName, ce.Line, ce.ErrorText));
                    hasError = true;
                }
            }
            if (hasError)
                Console.WriteLine("Failed to build assembly.");

            return cr.CompiledAssembly;
        }

        public static Dictionary<string,Sakura.BuildTools.BuildTarget> generateAssembliesFromTargets() {
            Dictionary<String,Sakura.BuildTools.BuildTarget> assemblies = new Dictionary<String,Sakura.BuildTools.BuildTarget>();
            foreach(string path in Directory.GetFiles("./", "*.cs", SearchOption.AllDirectories)) {
                if(path.EndsWith("Target.cs")) {
                    Assembly assembly = buildAssembly(path);
                    foreach(Type clazz in assembly.GetTypes()) {
                        if(clazz.IsSubclassOf(typeof(Sakura.BuildTools.BuildTarget)))
                        {
                            object instance = Activator.CreateInstance(clazz);
                            Sakura.BuildTools.BuildTarget target = (Sakura.BuildTools.BuildTarget)instance;
                            clazz.GetMethod("Build").Invoke(instance,new object[] {});
                            
                            List<string> includeDirectories = new List<string>();
                            List<string> libraryDirectories = new List<string>();
                            foreach(string include in target.GetIncludeDirectories()) {
                                includeDirectories.Add(Path.GetDirectoryName(Path.GetFullPath(path)) + "\\" + include);
                            }
                            foreach(string library in target.GetLibraryDirectories()) {
                                libraryDirectories.Add(Path.GetDirectoryName(Path.GetFullPath(path)) + "\\" + library);
                            }

                            target.includeDirectories = includeDirectories;
                            target.libraryDirectories = libraryDirectories;
                            target.Localization = path;
                            assemblies[path] = target;
                        }
                    }
                }
            }
            for(int i = 0;i<assemblies.Count;i++) {
                var entry = assemblies.ElementAt(i);
                Sakura.BuildTools.BuildTarget entryValue = entry.Value;
                if(entry.Value.GetModules().Count != 0) {
                    foreach(string module in entry.Value.GetModules()) {
                        //Find the module by name
                        Sakura.BuildTools.BuildTarget target = null;
                        for(int i1 = 0;i1<assemblies.Count;i1++) {
                            var entry1 = assemblies.ElementAt(i1);
                            if(entry1.Value.GetName().Equals(module)) {
                                target = entry1.Value;
                            }
                        }
                        if(target != null) {
                            foreach(string str in target.modules) {
                                entryValue.modules.Add(str);
                            }
                            assemblies[entry.Key] = entryValue;
                        }
                    }


                    //Reprocess after adding new modules
                    foreach(string module in entry.Value.GetModules()) {
                        //Find the module by name
                        Sakura.BuildTools.BuildTarget target = null;
                        for(int i1 = 0;i1<assemblies.Count;i1++) {
                            var entry1 = assemblies.ElementAt(i1);
                            if(entry1.Value.GetName().Equals(module)) {
                                target = entry1.Value;
                            }
                        }
                        if(target != null) {
                            entryValue.includeDirectories.Add(Path.GetDirectoryName(Path.GetFullPath(target.Localization)) + "\\" + target.GetSourceFolder());
                            foreach(string str in target.includeDirectories) {
                                entryValue.includeDirectories.Add(str);
                            }
                            foreach(string str in target.libraryDirectories) {
                                entryValue.libraryDirectories.Add(str);
                            }
                            foreach(string str in target.linkLibraries) {
                                entryValue.linkLibraries.Add(str);
                            }
                            assemblies[entry.Key] = entryValue;
                        }
                    }
                    
                }
            }
            return assemblies;
        }
    }
}