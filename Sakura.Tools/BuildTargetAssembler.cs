using System;
using System.Collections.Generic;
using System.CodeDom.Compiler;
using System.IO;
using System.Reflection;

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

        public static List<Sakura.BuildTools.BuildInfo> generateAssembliesFromTargets() {
            List<Sakura.BuildTools.BuildInfo> assemblies = new List<Sakura.BuildTools.BuildInfo>();
            foreach(string path in Directory.GetFiles("./", "*.cs", SearchOption.AllDirectories)) {
                if(path.EndsWith("Target.cs")) {
                    Assembly assembly = buildAssembly(path);
                    foreach(Type clazz in assembly.GetTypes()) {
                        if(clazz.IsSubclassOf(typeof(Sakura.BuildTools.BuildTarget)))
                        {
                            object instance = Activator.CreateInstance(clazz);
                            Sakura.BuildTools.BuildInfo info = (Sakura.BuildTools.BuildInfo)clazz.GetMethod("build").Invoke(instance,new object[] {});
                            assemblies.Add(info);
                        }
                    }
                }
            }
            return assemblies;
        }
    }
}